/*
 * DPF Max Gen
 * Copyright (C) 2015-2022 Filipe Coelho <falktx@falktx.com>
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

#ifndef DISTRHO_PLUGIN_MAX_GEN_HPP_INCLUDED
#define DISTRHO_PLUGIN_MAX_GEN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "genlib.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoPluginMaxGen : public Plugin
{
public:
    DistrhoPluginMaxGen();
    ~DistrhoPluginMaxGen() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return DISTRHO_PLUGIN_NAME;
    }

    const char* getDescription() const noexcept override
    {
        return DISTRHO_PLUGIN_DESCRIPTION;
    }

    int64_t getUniqueId() const noexcept override
    {
        return DISTRHO_PLUGIN_VERSION;
    }

    const char* getMaker() const noexcept override
    {
        return "DISTRHO";
    }

    const char* getHomePage() const noexcept override
    {
        return "https://github.com/DISTRHO/DPF-Max-Gen";
    }

    const char* getLicense() const noexcept override
    {
        return "ISC";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(0, 1, 0);
    }

    // -------------------------------------------------------------------
    // Init

    void initAudioPort(bool input, uint32_t index, AudioPort& port) override;
    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Process

    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    CommonState* fGenState;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoPluginMaxGen)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_MAX_GEN_HPP_INCLUDED
