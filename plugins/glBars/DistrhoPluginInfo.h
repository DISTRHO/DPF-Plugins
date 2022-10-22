/*
 * DISTRHO glBars Plugin based on XMMS/XBMC "GL Bars"
 * Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 * Copyright (C) 2000 Christian Zander <phoenix@minion.de>
 * Copyright (C) 2015 Nedko Arnaudov
 * Copyright (C) 2016-2022 Filipe Coelho <falktx@falktx.com>
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
 * For a full copy of the license see the LICENSE file.
 */

#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_BRAND   "DISTRHO"
#define DISTRHO_PLUGIN_NAME    "glBars"
#define DISTRHO_PLUGIN_URI     "http://distrho.sf.net/plugins/glBars"
#define DISTRHO_PLUGIN_CLAP_ID "studio.kx.distrho.glBars"

#define DISTRHO_PLUGIN_HAS_UI             1
#define DISTRHO_PLUGIN_NUM_INPUTS         1
#define DISTRHO_PLUGIN_NUM_OUTPUTS        1
#define DISTRHO_PLUGIN_WANT_DIRECT_ACCESS 1
#define DISTRHO_PLUGIN_CLAP_FEATURES      "analyzer", "stereo"
#define DISTRHO_PLUGIN_LV2_CATEGORY       "lv2:AnalyserPlugin"
#define DISTRHO_PLUGIN_VST3_CATEGORIES    "Fx|Analyzer"
#define DISTRHO_UI_USER_RESIZABLE         1

enum Parameters {
    kParameterScale = 0,
    kParameterSpeed,
    kParameterX,
    kParameterY,
    kParameterZ,
    kParameterCount
};

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
