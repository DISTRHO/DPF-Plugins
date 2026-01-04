/*
 * DISTRHO ProM Plugin
 * Copyright (C) 2015-2026 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * For a full copy of the license see the LICENSE file.
 */

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND   "DISTRHO"
#define DISTRHO_PLUGIN_NAME    "ProM"
#define DISTRHO_PLUGIN_URI     "http://distrho.sf.net/plugins/ProM"
#define DISTRHO_PLUGIN_CLAP_ID "studio.kx.distrho.ProM"

#define DISTRHO_PLUGIN_HAS_UI        1
#define DISTRHO_PLUGIN_NUM_INPUTS    2
#define DISTRHO_PLUGIN_NUM_OUTPUTS   2
#define DISTRHO_UI_USER_RESIZABLE    1

#define DISTRHO_UI_DEFAULT_WIDTH 512
#define DISTRHO_UI_DEFAULT_HEIGHT 512

// required by projectM
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS 1

#define DISTRHO_PLUGIN_CLAP_FEATURES   "analyzer", "stereo"
#define DISTRHO_PLUGIN_LV2_CATEGORY    "lv2:AnalyserPlugin"
#define DISTRHO_PLUGIN_VST3_CATEGORIES "Fx|Analyzer"

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
