/*
 * DISTRHO AmplitudeImposer, a DPF'ied AmplitudeImposer.
 * Copyright (C) 2004 Niall Moody
 * Copyright (C) 2015-2022 Filipe Coelho <falktx@falktx.com>
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

#ifndef DISTRHO_PLUGIN_AMPLITUDE_IMPOSER_HPP_INCLUDED
#define DISTRHO_PLUGIN_AMPLITUDE_IMPOSER_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoPluginAmplitudeImposer : public Plugin
{
public:
    enum Parameters {
        kParameterDepth,
        kParameterThreshold,
        kParameterCount
    };
    
    enum PortGroups {
        kPortGroupAmpEnv,
        kPortGroupAudio,
        kPortGroupCount
    };

    DistrhoPluginAmplitudeImposer();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getName() const noexcept override
    {
        return "Amplitude Imposer";
    }

    const char* getLabel() const noexcept override
    {
        return "AmplitudeImposer";
    }

    const char* getDescription() const override
    {
        return "Takes 2 stereo inputs and imposes the amplitude envelope of the first one on the second one.\n\
Also has a threshold level for the second input, so that when the signal falls below it, it is amplified up to the threshold, to give a greater signal to be amplitude modulated.";
    }

    const char* getMaker() const noexcept override
    {
        return "ndc Plugs";
    }

    const char* getHomePage() const override
    {
        return "https://github.com/DISTRHO/ndc-Plugs";
    }

    const char* getLicense() const noexcept override
    {
        return "MIT";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(0, 1, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('A', 'm', 'I', 'm');
    }

    // -------------------------------------------------------------------
    // Init

    void initAudioPort(bool input, uint32_t index, AudioPort& port) override;
    void initParameter(uint32_t index, Parameter& parameter) override;
    void initPortGroup(uint32_t groupId, PortGroup& portGroup) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Process

    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    float fDepth;
    float fThreshold;

    float ampEnvelope_l, ampEnvelope_r;
    float audioEnvelope_l, audioEnvelope_r;
    float envDecay;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoPluginAmplitudeImposer)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_AMPLITUDE_IMPOSER_HPP_INCLUDED
