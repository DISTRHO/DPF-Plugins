/*
 * DISTRHO ProM Plugin
 * Copyright (C) 2015-2022 Filipe Coelho <falktx@falktx.com>
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

#include "DistrhoPluginProM.hpp"

#include "libprojectM/projectM.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginProM::DistrhoPluginProM()
    : Plugin(0, 0, 0),
      fPM(nullptr)
{
}

DistrhoPluginProM::~DistrhoPluginProM()
{
    DISTRHO_SAFE_ASSERT(fPM == nullptr);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginProM::initAudioPort(bool input, uint32_t index, AudioPort& port)
{
    port.groupId = kPortGroupStereo;

    Plugin::initAudioPort(input, index, port);
}

void DistrhoPluginProM::initParameter(uint32_t, Parameter&)
{
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginProM::getParameterValue(uint32_t) const
{
    return 0.0f;
}

void DistrhoPluginProM::setParameterValue(uint32_t, float)
{
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginProM::run(const float** inputs, float** outputs, uint32_t frames)
{
    const float* in1  = inputs[0];
    const float* in2  = inputs[1];
    float*       out1 = outputs[0];
    float*       out2 = outputs[1];

    if (out1 != in1)
        std::memcpy(out1, in1, sizeof(float)*frames);
    if (out2 != in2)
        std::memcpy(out2, in2, sizeof(float)*frames);

    const MutexLocker csm(fMutex);

    if (fPM == nullptr)
        return;

    if (PCM* const pcm = const_cast<PCM*>(fPM->pcm()))
        pcm->addPCMfloat(in1, frames);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginProM();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
