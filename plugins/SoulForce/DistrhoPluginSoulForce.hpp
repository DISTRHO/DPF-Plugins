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

#ifndef DISTRHO_PLUGIN_SOUL_FORCE_HPP_INCLUDED
#define DISTRHO_PLUGIN_SOUL_FORCE_HPP_INCLUDED

#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoPluginSoulForce : public Plugin
{
public:
    enum Parameters {
        kParameterShape,
        kParameterFeedback,
        kParameterSource,
        kParameterFootswitch,
        kParameterCount
    };

    enum Programs {
        kProgramDefault,
        kProgramStayDown,
        kProgramLookingForTheWorld,
        kProgramGuerillaLove,
        kProgramTumbleToThePower,
        kProgramDoYourselfFavour,
        kProgramPastIsPast,
        kProgramYouAndOnlyYou,
        kProgramSoulForce,
        kProgramCount
    };

    DistrhoPluginSoulForce();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "SoulForce";
    }

    const char* getMaker() const noexcept override
    {
        return "ndc Plugs";
    }

    const char* getLicense() const noexcept override
    {
        return "MIT";
    }

    uint32_t getVersion() const noexcept override
    {
        return 0x1000;
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('S', 'l', 'F', 'r');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;
    void initProgramName(uint32_t index, String& programName) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;
    void  loadProgram(uint32_t index) override;

    // -------------------------------------------------------------------
    // Process

    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    /// Array of our plugin's parameters.
    float parameters[kParameterCount];

    /// Waveshaper coefficient.
    float coeff;
    /// Current wave value for the feedback.
    float wave;
    /// Envelope value for the feedback.
    float env;
    /// Footswitch envelope.
    float footEnv;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoPluginSoulForce)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_SOUL_FORCE_HPP_INCLUDED
