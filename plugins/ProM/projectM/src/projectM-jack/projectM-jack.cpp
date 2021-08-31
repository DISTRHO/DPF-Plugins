/**
 * projectM -- Milkdrop-esque visualisation SDK
 * Copyright (C)2003-2004 projectM Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * See 'LICENSE.txt' included within this release
 *
 */
#include "video_init.h"
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <projectM.hpp>
#include "sdltoprojectM.h"

#include "ConfigFile.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include <jack/jack.h>

#define CONFIG_FILE "/share/projectM/config.inp"

std::string read_config();

jack_port_t *input_port;
jack_client_t *client;
jack_default_audio_sample_t out[1024 * 1000];

SDL_Window *screen;
SDL_Renderer *renderer;

#ifdef DEBUG
FILE *debugFile = NULL;
#endif

volatile enum client_state_t { Init, Run, Exit } client_state = Init;

projectM *globalPM = NULL;

int dumpFrame = 0;
int frameNumber = 0;

int texsize = 512;
int gx = 32, gy = 24;
int wvw = 512, wvh = 512;
int fvw = 1024, fvh = 768;
int fps = 30, fullscreen = 0;

std::string read_config()
{

	int n;

	char num[512];
	FILE *in;
	FILE *out;

	char *home;
	char projectM_home[1024];
	char projectM_config[1024];

	strcpy(projectM_config, PROJECTM_PREFIX);
	strcpy(projectM_config + strlen(PROJECTM_PREFIX), CONFIG_FILE);
	projectM_config[strlen(PROJECTM_PREFIX) + strlen(CONFIG_FILE)] = '\0';
	printf("dir:%s \n", projectM_config);
	home = getenv("HOME");
	strcpy(projectM_home, home);
	strcpy(projectM_home + strlen(home), "/.projectM/config.inp");
	projectM_home[strlen(home) + strlen("/.projectM/config.inp")] = '\0';

	if ((in = fopen(projectM_home, "r")) != 0)
	{
		printf("reading ~/.projectM/config.inp \n");
		fclose(in);
		return std::string(projectM_home);
	}
	else
	{
		printf("trying to create ~/.projectM/config.inp \n");

		strcpy(projectM_home, home);
		strcpy(projectM_home + strlen(home), "/.projectM");
		projectM_home[strlen(home) + strlen("/.projectM")] = '\0';
		mkdir(projectM_home, 0755);

		strcpy(projectM_home, home);
		strcpy(projectM_home + strlen(home), "/.projectM/config.inp");
		projectM_home[strlen(home) + strlen("/.projectM/config.inp")] = '\0';

		if ((out = fopen(projectM_home, "w")) != 0)
		{

			if ((in = fopen(projectM_config, "r")) != 0)
			{

				while (fgets(num, 80, in) != NULL)
				{
					fputs(num, out);
				}
				fclose(in);
				fclose(out);

				if ((in = fopen(projectM_home, "r")) != 0)
				{
					printf("created ~/.projectM/config.inp successfully\n");
					fclose(in);
					return std::string(projectM_home);
				}
				else
				{
					printf("This shouldn't happen, using implementation defaults\n");
					abort();
				}
			}
			else
			{
				printf("Cannot find projectM default config, using implementation defaults\n");
				abort();
			}
		}
		else
		{
			printf("Cannot create ~/.projectM/config.inp, using default config file\n");
			if ((in = fopen(projectM_config, "r")) != 0)
			{
				printf("Successfully opened default config file\n");
				fclose(in);
				return std::string(projectM_config);
			}
			else
			{
				printf(
					"Using implementation defaults, your system is really messed up, I'm surprised we even "
					"got this far\n");
				abort();
			}
		}
	}
}

void renderLoop()
{

	int i;
	int x, y;
	int index;
	short pcm_data[2][512];

	while (1)
	{
		projectMEvent evt;
		projectMKeycode key;
		projectMModifier mod;

		/** Process SDL events */
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			/** Translate into projectM codes and process */
			evt = sdl2pmEvent(event);
			key = sdl2pmKeycode(event.key.keysym.sym);
			mod = sdl2pmModifier(event.key.keysym.mod);

			if (evt == PROJECTM_KEYDOWN)
			{

				if (key == PROJECTM_K_f)
				{
					if (++fullscreen % 2)
					{
						resize_display(fvw, fvh, fullscreen % 2);
						globalPM->projectM_resetGL(fvw, fvh);
					}
					else
					{
						resize_display(wvw, wvh, fullscreen % 2);
						globalPM->projectM_resetGL(wvw, wvh);
					}
				}
				else if (key == PROJECTM_K_q || key == PROJECTM_K_ESCAPE)
				{
					delete (globalPM);
					exit(1);
				}
				else
				{
					globalPM->key_handler(evt, key, mod);
				}
			}
			else if (evt == PROJECTM_VIDEORESIZE)
			{
				wvw = event.window.data1;
				wvh = event.window.data2;
				resize_display(wvw, wvh, fullscreen % 2);
				globalPM->projectM_resetGL(wvw, wvh);
			}
		}

		/** Render the new frame */
		globalPM->renderFrame();

		SDL_GL_SwapWindow(screen);
	}

	printf("Worker thread: Exiting\n");
}

int process(jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t *in;

	in = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port, nframes);

	// memcpy (out, in,sizeof (jack_default_audio_sample_t) * nframes);

	globalPM->pcm()->addPCMfloat(in, nframes);
	// printf("%x %f\n",nframes,in[128]);

	return 0;
}

void jack_shutdown(void *arg)
{
	exit(1);
}

int main(int argc, char **argv)
{
	const char **ports;
	const char *client_name;
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;
	int i;
	char projectM_data[1024];

	std::string config_file;
	config_file = read_config();

	ConfigFile config(config_file);

	wvw = config.read<int>("Window Width", 512);
	wvh = config.read<int>("Window Height", 512);
	int fullscreen = 0;
	if (config.read("Fullscreen", true))
		fullscreen = 1;
	else
		fullscreen = 0;

#ifdef DEBUG
	int value;
	int rgb_size[3];
#endif

	int bpp = 0;
	/* Flags we will pass into SDL_SetVideoMode. */
	int flags = 0;

	// JACK INIT
	//----------------------------------------------
	if (argc >= 2)
	{ /* client name specified? */
		client_name = argv[1];
		if (argc >= 3)
		{ /* server name specified? */
			server_name = argv[2];
			//			options |= JackServerName;
		}
	}
	else
	{ /* use basename of argv[0] */
		client_name = strrchr(argv[0], '/');
		if (client_name == 0)
		{
			client_name = argv[0];
		}
		else
		{
			client_name++;
		}
	}

	/* open a client connection to the JACK server */

	client = jack_client_open(client_name, options, &status, server_name);
	if (client == NULL)
	{
		fprintf(stderr,
			"jack_client_open() failed, "
			"status = 0x%2.0x\n",
			status);
		if (status & JackServerFailed)
		{
			fprintf(stderr, "Unable to connect to JACK server\n");
		}
		exit(1);
	}
	if (status & JackServerStarted)
	{
		fprintf(stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique)
	{
		client_name = jack_get_client_name(client);
		fprintf(stderr, "unique name `%s' assigned\n", client_name);
	}

	/* tell the JACK server to call `process()' whenever
		 there is work to be done.
	*/

	jack_set_process_callback(client, process, 0);

	/* tell the JACK server to call `jack_shutdown()' if
		 it ever shuts down, either entirely, or if it
		 just decides to stop calling us.
	*/

	jack_on_shutdown(client, jack_shutdown, 0);

	/* display the current sample rate.
	 */

	printf("engine sample rate: %d\n", jack_get_sample_rate(client));

	/* create two ports */

	input_port = jack_port_register(client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	if (input_port == NULL)
	{
		fprintf(stderr, "no more JACK ports available\n");
		exit(1);
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	// END JACK INIT ----------------------------------
	init_display(wvw, wvh, &fvw, &fvh, fullscreen);
	/** Setup some window stuff */
	globalPM = new projectM(config_file);
	/** Initialise projectM */

	// JACK BEGIN-----------------------------

	if (jack_activate(client))
	{
		fprintf(stderr, "cannot activate client");
		exit(1);
	}

	/* Connect the ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */

	ports = jack_get_ports(client, NULL, NULL, JackPortIsOutput);
	if (ports == NULL)
	{
		fprintf(stderr, "no physical capture ports\n");
		exit(1);
	}

	i = 0;
	while (ports[i] != NULL)
	{
		printf("Connecting to Jack port %s\n", ports[i]);
		if (jack_connect(client, ports[i], jack_port_name(input_port)))
		{
			fprintf(stderr, "cannot connect input ports\n");
		}
		i++;
	}

	free(ports);

	//----------------------------------END

	/** Initialise the thread */
	renderLoop();

	close_display();

	return 0;
}
