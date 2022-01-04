/*
 * DISTRHO CycleShifter, a DPF'ied CycleShifter.
 * Copyright (C) 2004 Niall Moody
 * Copyright (C) 2015 Filipe Coelho <falktx@falktx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "DistrhoPluginCycleShifter.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginCycleShifter::DistrhoPluginCycleShifter()
    : Plugin(kParameterCount, 1, 0), // 1 program, 0 states
      fNewCycleVolume(1.0f),
      fInputVolume(1.0f),
      OutIndex(0),
      InCount(0),
      ReadWrite(false),
      EnvOld(0.0f)
{
    std::memset(CycleBuffer, 0, sizeof(float)*BUFFER_SIZE);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginCycleShifter::initParameter(uint32_t index, Parameter& parameter)
{
    parameter.hints      = kParameterIsAutomatable;
    parameter.ranges.min = 0.0f;
    parameter.ranges.max = 1.0f;

    switch (index)
    {
    case kParameterNewCycleVolume:
        parameter.name       = "New Cycle Vol";
        parameter.symbol     = "ncvolume";
        parameter.ranges.def = 1.0f;
        break;
    case kParameterInputVolume:
        parameter.name       = "Input Vol";
        parameter.symbol     = "ipvolume";
        parameter.ranges.def = 1.0f;
        break;
    }
}

void DistrhoPluginCycleShifter::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginCycleShifter::getParameterValue(uint32_t index) const
{
    switch(index)
    {
    case kParameterNewCycleVolume:
        return fNewCycleVolume;
    case kParameterInputVolume:
        return fInputVolume;
    default:
        return 0.0f;
    }
}

void DistrhoPluginCycleShifter::setParameterValue(uint32_t index, float value)
{
    switch(index)
    {
    case kParameterNewCycleVolume:
        fNewCycleVolume = value;
        break;
    case kParameterInputVolume:
        fInputVolume = value;
        break;
    }
}

void DistrhoPluginCycleShifter::loadProgram(uint32_t index)
{
    if (index != 0)
        return;

    fNewCycleVolume = 1.0f;
    fInputVolume    = 1.0f;
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginCycleShifter::activate()
{
    std::memset(CycleBuffer, 0, sizeof(float)*BUFFER_SIZE);
    OutIndex = 0;
    InCount = 0;
    ReadWrite = false;
    EnvOld = 0.0f;
}

void DistrhoPluginCycleShifter::run(const float** inputs, float** outputs, uint32_t frames)
{
    const float* in  =  inputs[0];
    /**/  float* out = outputs[0];

    for (uint32_t i=0; i<frames; ++i)
        *out++ = DoProcess(*in++);
}

// -----------------------------------------------------------------------
// Borrowed this from Toby Bear's Delphi template - it maybe adds a bit to cpu
// usage, but it makes things simpler...
// -----------------------------------------------------------------------

float DistrhoPluginCycleShifter::DoProcess(float a)
{
    const float tempval = a;

    if (! ReadWrite) // if we're in read mode
    {
        if (InCount == 0) // if we're waiting for the start of a new cycle to read
        {
            if (EnvOld < 0.0f && tempval >= 0.0f) // as soon as the input goes past 0 we start reading
            {
                CycleBuffer[InCount++] = tempval;
            }
        }
        else if (! (EnvOld < 0.0f && tempval >= 0.0f)) // if we've not reached the end of the cycle yet
        {
            CycleBuffer[InCount] = tempval;
            if (++InCount >= BUFFER_SIZE) // if we've reached the end of the buffer
            {
                InCount = BUFFER_SIZE;
                ReadWrite = true; // we're in write mode now
            }
        }
        else // we've reached the end of the cycle
        {
            CycleBuffer[InCount++] = 0.0f;
            ReadWrite = true;
        }
        a *= fInputVolume;
    }
    else // we're in write mode
    {
        a = (a*fInputVolume) + (CycleBuffer[OutIndex]*fNewCycleVolume);
        if (++OutIndex == InCount) // we've reached the end of our stored cycle
        {
            InCount = 0;
            OutIndex = 0;
            ReadWrite = false;
        }
    }

    EnvOld = tempval;

    return a;
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginCycleShifter();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
