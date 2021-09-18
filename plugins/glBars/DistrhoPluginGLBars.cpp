/*
 * DISTRHO glBars Plugin based on XMMS/XBMC "GL Bars"
 * Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 * Copyright (C) 2000 Christian Zander <phoenix@minion.de>
 * Copyright (C) 2015 Nedko Arnaudov
 * Copyright (C) 2016 Filipe Coelho <falktx@falktx.com>
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

#include "DistrhoPluginGLBars.hpp"

#include "glBars.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginGLBars::DistrhoPluginGLBars()
    : Plugin(kParameterCount, 0, 0),
      fState(nullptr)
{
    fParameters[kParameterScale] = 1.f / log(256.f);
    fParameters[kParameterSpeed] = 0.025f;
    fParameters[kParameterX]     = 0.0f;
    fParameters[kParameterY]     = 0.5f;
    fParameters[kParameterZ]     = 0.0f;
}

DistrhoPluginGLBars::~DistrhoPluginGLBars()
{
    DISTRHO_SAFE_ASSERT(fState == nullptr);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginGLBars::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
    case kParameterScale:
        parameter.hints      = kParameterIsAutomable|kParameterIsLogarithmic;
        parameter.name       = "Scale";
        parameter.symbol     = "scale";
        parameter.unit       = "";
        parameter.ranges.def = 1.0f / log(256.f);
        parameter.ranges.min = 0.5f / log(256.f);
        parameter.ranges.max = 3.0f / log(256.f);
        break;

    case kParameterSpeed:
        parameter.hints      = kParameterIsAutomable|kParameterIsLogarithmic;
        parameter.name       = "Speed";
        parameter.symbol     = "speed";
        parameter.unit       = "";
        parameter.ranges.def = 0.025f;
        parameter.ranges.min = 0.0125f;
        parameter.ranges.max = 0.1f;
        break;

    case kParameterX:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "X";
        parameter.symbol     = "x";
        parameter.unit       = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -4.0f;
        parameter.ranges.max = 4.0f;
        break;

    case kParameterY:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Y";
        parameter.symbol     = "y";
        parameter.unit       = "";
        parameter.ranges.def = 0.5f;
        parameter.ranges.min = -4.0f;
        parameter.ranges.max = 4.0f;
        break;

    case kParameterZ:
        parameter.hints      = kParameterIsAutomable;
        parameter.name       = "Z";
        parameter.symbol     = "z";
        parameter.unit       = "";
        parameter.ranges.def = 0.0f;
        parameter.ranges.min = -4.0f;
        parameter.ranges.max = 4.0f;
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginGLBars::getParameterValue(uint32_t index) const
{
    if (index >= kParameterCount)
        return 0.0f;

    return fParameters[index];
}

void DistrhoPluginGLBars::setParameterValue(uint32_t index, float value)
{
    if (index >= kParameterCount)
        return;

    fParameters[index] = value;
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginGLBars::run(const float** inputs, float** outputs, uint32_t frames)
{
    const float* in  = inputs[0];
    float*       out = outputs[0];

    if (out != in)
        std::memcpy(out, in, sizeof(float)*frames);

    const MutexLocker csm(fMutex);

    if (fState != nullptr)
        fState->AudioData(in, frames);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginGLBars();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
