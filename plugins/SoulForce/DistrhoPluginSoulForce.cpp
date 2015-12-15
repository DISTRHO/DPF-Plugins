/*
 * DISTRHO SoulForce, a DPF'ied SoulForce.
 * Copyright (C) 2006 Niall Moody
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

#include "DistrhoPluginSoulForce.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginSoulForce::DistrhoPluginSoulForce()
    : Plugin(kParameterCount, kProgramCount, 0),
      coeff(0.5f),
      wave(0.0f),
      env(0.0f),
      footEnv(0.0f)
{
    // set initial values
    loadProgram(0);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginSoulForce::initParameter(uint32_t index, Parameter& parameter)
{
    parameter.hints      = kParameterIsAutomable;
    parameter.ranges.min = 0.0f;
    parameter.ranges.max = 1.0f;

    switch (index)
    {
    case kParameterShape:
        parameter.name       = "Shape";
        parameter.symbol     = "shape";
        parameter.ranges.def = 0.5f;
        break;
    case kParameterFeedback:
        parameter.name       = "FBack";
        parameter.symbol     = "fback";
        parameter.ranges.def = 0.0f;
        break;
    case kParameterSource:
        parameter.hints     |= kParameterIsBoolean;
        parameter.name       = "Source";
        parameter.symbol     = "source";
        parameter.ranges.def = 0.0f;
        break;
    case kParameterFootswitch:
        parameter.hints     |= kParameterIsBoolean;
        parameter.name       = "Foot";
        parameter.symbol     = "foot";
        parameter.ranges.def = 1.0f;
        break;
    }
}

void DistrhoPluginSoulForce::initProgramName(uint32_t index, String& programName)
{
    switch(index)
    {
    case kProgramDefault:
        programName = "Default";
        break;
    case kProgramStayDown:
        programName = "Stay Down";
        break;
    case kProgramLookingForTheWorld:
        programName = "Looking for the World";
        break;
    case kProgramGuerillaLove:
        programName = "Guerilla Love";
        break;
    case kProgramTumbleToThePower:
        programName = "Tumble to the Power";
        break;
    case kProgramDoYourselfFavour:
        programName = "Do Yourself a Favour";
        break;
    case kProgramPastIsPast:
        programName = "Past is Past";
        break;
    case kProgramYouAndOnlyYou:
        programName = "You and Only You";
        break;
    case kProgramSoulForce:
        programName = "Soul Force";
        break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginSoulForce::getParameterValue(uint32_t index) const
{
    return parameters[index];
}

void DistrhoPluginSoulForce::setParameterValue(uint32_t index, float value)
{
    parameters[index] = value;

    switch(index)
    {
    case kParameterShape:
        coeff = (1.0f-(1.0f/((value * 0.99f)+0.005f)));
        break;
    case kParameterFeedback:
        if (d_isZero(value))
            coeff = (1.0f-(1.0f/((parameters[kParameterShape] * 0.99f)+0.005f)));
        break;
    }
}

void DistrhoPluginSoulForce::loadProgram(uint32_t index)
{
    switch(index)
    {
    case kProgramDefault:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramStayDown:
        parameters[kParameterShape]      = 0.4f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramLookingForTheWorld:
        parameters[kParameterShape]      = 1.0f;
        parameters[kParameterFeedback]   = 0.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramGuerillaLove:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramTumbleToThePower:
        parameters[kParameterShape]      = 0.0f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramDoYourselfFavour:
        parameters[kParameterShape]      = 0.5f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramPastIsPast:
        parameters[kParameterShape]      = 0.0f;
        parameters[kParameterFeedback]   = 1.0f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramYouAndOnlyYou:
        parameters[kParameterShape]      = 0.3f;
        parameters[kParameterFeedback]   = 0.5f;
        parameters[kParameterSource]     = 0.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    case kProgramSoulForce:
        parameters[kParameterShape]      = 0.3f;
        parameters[kParameterFeedback]   = 0.5f;
        parameters[kParameterSource]     = 1.0f;
        parameters[kParameterFootswitch] = 1.0f;
        break;
    }
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginSoulForce::run(const float** inputs, float** outputs, uint32_t frames)
{
    float tempf, tempf2;
    float inLeft, inRight;

    //frames = sampleFrames;

    // Calculate audio.
    for (uint32_t i=0; i<frames; ++i)
    {
        // For footEnv, later.
        inLeft  = inputs[0][i];
        inRight = inputs[1][i];

        // Update coeff, if necessary.
        if (parameters[kParameterFeedback] > 0.0f)
        {
            if (parameters[kParameterSource] > 0.5f)
            {
                tempf2  = (1.0f-parameters[kParameterFeedback]) * parameters[kParameterShape];
                tempf2 += parameters[kParameterFeedback] * wave;
                coeff   = (1.0f-(1.0f/((tempf2 * 0.99f)+0.005f)));
            }
            else
            {
                if ((tempf = std::abs(inLeft)) > env)
                {
                    env = tempf;
                }
                else if (env > 0.0f)
                {
                    env -= 0.001f;
                    if (env < 0.0f)
                        env = 0.0f;
                }

                tempf2  = (1.0f-parameters[kParameterFeedback]) * parameters[kParameterShape];
                tempf2 += parameters[kParameterFeedback] * env;
                coeff   = (1.0f-(1.0f/((tempf2 * 0.99f)+0.005f)));
            }
        }

        tempf = inLeft;
        if (tempf > 0.0f)
        {
            tempf = tempf/(tempf+(coeff*(tempf-1.0f)));
        }
        else if (d_isZero(tempf))
        {
            tempf = 0.0f;
        }
        else
        {
            tempf  = std::abs(tempf);
            tempf  = 1.0f - (tempf/(tempf+(coeff*(tempf-1.0f))));
            tempf -= 1.0f;
        }
        if (parameters[kParameterShape] < 0.5f)
            tempf *= ((0.5f-parameters[kParameterShape])*16.0f)+1.0f;
        if (parameters[kParameterSource] > 0.5f)
            tempf *= 1.0f + (parameters[kParameterFeedback] * 2.0f);
        outputs[0][i] = tempf;

        tempf = inRight;
        if (tempf > 0.0f)
        {
            tempf = tempf/(tempf+(coeff*(tempf-1.0f)));
        }
        else if (d_isZero(tempf))
        {
            tempf = 0.0f;
        }
        else
        {
            tempf  = std::abs(tempf);
            tempf  = 1.0f - (tempf/(tempf+(coeff*(tempf-1.0f))));
            tempf -= 1.0f;
        }
        if (parameters[kParameterShape] < 0.5f)
            tempf *= ((0.5f-parameters[kParameterShape])*16.0f)+1.0f;
        if (parameters[kParameterSource] > 0.5f)
            tempf *= 1.0f + (parameters[kParameterFeedback] * 2.0f);
        outputs[1][i] = tempf;

        outputs[0][i] = std::tanh(outputs[0][i]) * 0.9f;
        outputs[1][i] = std::tanh(outputs[1][i]) * 0.9f;
        wave = outputs[0][i];

        // Handle footswitch.
        outputs[0][i] *= footEnv;
        outputs[0][i] += (1.0f-footEnv) * inLeft;
        outputs[1][i] *= footEnv;
        outputs[1][i] += (1.0f-footEnv) * inRight;

        if (parameters[kParameterFootswitch] > 0.5f && footEnv < 1.0f)
            footEnv += 0.01f;
        else if (parameters[kParameterFootswitch] < 0.5f && footEnv > 0.0f)
            footEnv -= 0.01f;
    }
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginSoulForce();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
