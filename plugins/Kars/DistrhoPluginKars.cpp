/*
 * DISTRHO Kars Plugin, based on karplong by Chris Cannam.
 * Copyright (C) 2015-2022 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPluginKars.hpp"
#include "DistrhoPluginUtils.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginKars::DistrhoPluginKars()
    : Plugin(paramCount, 0, 0), // 0 programs, 0 states
      fSustain(false),
      fRelease(0.01),
      fVolume(75.0f),
      fSampleRate(getSampleRate()),
      fBlockStart(0)
{
    for (int i=kMaxNotes; --i >= 0;)
    {
        fNotes[i].voice = i;
        fNotes[i].setSampleRate(fSampleRate);
    }
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginKars::initAudioPort(bool input, uint32_t index, AudioPort& port)
{
    port.groupId = kPortGroupMono;

    Plugin::initAudioPort(input, index, port);
}

void DistrhoPluginKars::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case paramSustain:
        parameter.hints      = kParameterIsAutomatable|kParameterIsBoolean;
        parameter.name       = "Sustain";
        parameter.symbol     = "sustain";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 1.0f;
        break;
    case paramRelease:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Release";
        parameter.symbol     = "release";
        parameter.unit       = "s";
        parameter.ranges.def = 0.01f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 5.0f;
        break;
    case paramVolume:
        parameter.hints      = kParameterIsAutomatable;
        parameter.name       = "Volume";
        parameter.symbol     = "volume";
        parameter.unit       = "%";
        parameter.ranges.def = 75.0f;
        parameter.ranges.min = 0.0f;
        parameter.ranges.max = 100.0f;
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginKars::getParameterValue(uint32_t index) const
{
    switch (index)
    {
    case paramSustain: return fSustain ? 1.0f : 0.0f;
    case paramRelease: return fRelease;
    case paramVolume:  return fVolume;
    }

    return 0.0f;
}

void DistrhoPluginKars::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
    case paramSustain:
        fSustain = value > 0.5f;
        break;
    case paramRelease:
        fRelease = value;
        break;
    case paramVolume:
        fVolume = value;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginKars::activate()
{
    fBlockStart = 0;

    for (int i=kMaxNotes; --i >= 0;)
    {
        fNotes[i].on  = kNoteNull;
        fNotes[i].off = kNoteNull;
        fNotes[i].velocity = 0;
    }
}

void DistrhoPluginKars::run(const float**, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount)
{
    uint8_t note, velo;
    std::memset(outputs[0], 0, sizeof(float)*frames);

    for (AudioMidiSyncHelper amsh(outputs, frames, midiEvents, midiEventCount); amsh.nextEvent();)
    {
        for (uint32_t i=0; i<amsh.midiEventCount; ++i)
        {
            if (amsh.midiEvents[i].size > MidiEvent::kDataSize)
                continue;

            const uint8_t*  data = amsh.midiEvents[i].data;
            const uint8_t status = data[0] & 0xF0;

            switch (status)
            {
            case 0x90:
                note = data[1];
                velo = data[2];
                DISTRHO_SAFE_ASSERT_BREAK(note < 128); // kMaxNotes
                if (velo > 0)
                {
                    fNotes[note].on  = fBlockStart;
                    fNotes[note].off = kNoteNull;
                    fNotes[note].velocity = velo;
                    break;
                }
                // fall through
            case 0x80:
                note = data[1];
                DISTRHO_SAFE_ASSERT_BREAK(note < 128); // kMaxNotes
                fNotes[note].off = fBlockStart;
                break;
            }
        }

        float* const out = amsh.outputs[0];

        for (int i=kMaxNotes; --i >= 0;)
        {
            if (fNotes[i].on != kNoteNull)
                addSamples(out, i, amsh.frames);
        }

        fBlockStart += amsh.frames;
    }
}

void DistrhoPluginKars::sampleRateChanged(double newSampleRate)
{
    fSampleRate = getSampleRate();

    for (int i=kMaxNotes; --i >= 0;)
        fNotes[i].setSampleRate(newSampleRate);
}

void DistrhoPluginKars::addSamples(float* out, int voice, uint32_t frames)
{
    const uint32_t start = fBlockStart;

    Note& note(fNotes[voice]);

    if (start < note.on)
        return;

    if (start == note.on)
    {
        for (int i=note.sizei; --i >= 0;)
            note.wavetable[i] = (float(rand()) / float(RAND_MAX)) * 2.0f - 1.0f;
    }

    const float vgain = float(note.velocity) / 127.0f;

    bool decay;
    float gain, sample;
    uint32_t index, size;

    for (uint32_t i=0, s=start-note.on; i<frames; ++i, ++s)
    {
        gain = vgain;

        if ((! fSustain) && note.off != kNoteNull && note.off < i+start)
        {
            // reuse index and size to save some performance.
            // actual values are release and dist
            index = 1 + uint32_t(fRelease * fSampleRate); // release, not index
            size  = i + start - note.off;                 // dist, not size

            if (size > index)
            {
                note.on = kNoteNull;
                break;
            }

            gain = gain * float(index - size) / float(index);
        }

        size  = uint32_t(note.sizei);
        decay = s > size;
        index = s % size;

        sample = note.wavetable[index];

        if (decay)
        {
            if (index == 0)
                sample += note.wavetable[size-1];
            else
                sample += note.wavetable[index-1];

            note.wavetable[index] = sample/2;
        }

        out[i] += gain * sample * (fVolume / 100.0f);
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginKars();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
