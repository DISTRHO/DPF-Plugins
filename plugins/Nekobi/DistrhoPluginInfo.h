/*
 * DISTRHO Nekobi Plugin, based on Nekobee by Sean Bolton and others.
 * Copyright (C) 2013-2024 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the LICENSE file.
 */

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#include "DistrhoArtworkNekobi.hpp"

#define DISTRHO_PLUGIN_BRAND   "DISTRHO"
#define DISTRHO_PLUGIN_NAME    "Nekobi"
#define DISTRHO_PLUGIN_URI     "http://distrho.sf.net/plugins/Nekobi"
#define DISTRHO_PLUGIN_CLAP_ID "studio.kx.distrho.Nekobi"

#define DISTRHO_PLUGIN_BRAND_ID  Dstr
#define DISTRHO_PLUGIN_UNIQUE_ID DNek

#define DISTRHO_PLUGIN_HAS_UI        1
#define DISTRHO_PLUGIN_IS_RT_SAFE    1
#define DISTRHO_PLUGIN_IS_SYNTH      1
#define DISTRHO_PLUGIN_NUM_INPUTS    0
#define DISTRHO_PLUGIN_NUM_OUTPUTS   1

#define DISTRHO_UI_DEFAULT_WIDTH DistrhoArtworkNekobi::backgroundWidth
#define DISTRHO_UI_DEFAULT_HEIGHT DistrhoArtworkNekobi::backgroundHeight

#define DPF_VST3_DONT_USE_BRAND_ID

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
