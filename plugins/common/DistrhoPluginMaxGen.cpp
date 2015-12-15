/*
 * DPF Max Gen
 * Copyright (C) 2015 Filipe Coelho <falktx@falktx.com>
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

#include "DistrhoPluginMaxGen.hpp"

#include "gen_exported.h"

namespace gen = gen_exported;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoPluginMaxGen::DistrhoPluginMaxGen()
    : Plugin(gen::num_params(), 0, 0), // 0 programs, 0 states
      fGenState((CommonState*)gen::create(getSampleRate(), getBufferSize()))
{
    gen::reset(fGenState);
}

DistrhoPluginMaxGen::~DistrhoPluginMaxGen()
{
    gen::destroy(fGenState);
}

// -----------------------------------------------------------------------
// Init

void DistrhoPluginMaxGen::initParameter(uint32_t index, Parameter& parameter)
{
    ParamInfo& info(fGenState->params[index]);

    parameter.hints      = kParameterIsAutomable;
    parameter.name       = info.name;
    parameter.symbol     = info.name;
    parameter.unit       = info.units;
    parameter.ranges.def = info.defaultvalue;
    parameter.ranges.min = info.outputmin;
    parameter.ranges.max = info.outputmax;
}

// -----------------------------------------------------------------------
// Internal data

float DistrhoPluginMaxGen::getParameterValue(uint32_t index) const
{
    double value = 0.0;
    gen::getparameter(fGenState, index, &value);
    return value;
}

void DistrhoPluginMaxGen::setParameterValue(uint32_t index, float value)
{
    gen::setparameter(fGenState, index, value, nullptr);
}

// -----------------------------------------------------------------------
// Process

void DistrhoPluginMaxGen::run(const float** inputs, float** outputs, uint32_t frames)
{
    gen::perform(fGenState, (float**)inputs, DISTRHO_PLUGIN_NUM_INPUTS, outputs, DISTRHO_PLUGIN_NUM_OUTPUTS, frames);
}

// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new DistrhoPluginMaxGen();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#include "gen_dsp/genlib.cpp"
