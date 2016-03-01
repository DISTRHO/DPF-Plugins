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

#ifndef DISTRHO_PLUGIN_GLBARS_HPP_INCLUDED
#define DISTRHO_PLUGIN_GLBARS_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "extra/Mutex.hpp"

class DistrhoUIGLBars;
struct glBarsState;

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoPluginGLBars : public Plugin
{
public:
    DistrhoPluginGLBars();
    ~DistrhoPluginGLBars() override;

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "glBars";
    }

    const char* getDescription() const override
    {
        return "OpenGL Bars";
    }

    const char* getMaker() const noexcept override
    {
        return "DISTRHO";
    }

    const char* getHomePage() const override
    {
        return "https://github.com/DISTRHO/GLBars";
    }

    const char* getLicense() const noexcept override
    {
        return "LGPL";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('D', 'g', 'l', 'B');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t, Parameter&) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t) const override;
    void  setParameterValue(uint32_t, float) override;

    // -------------------------------------------------------------------
    // Process

    void run(const float** inputs, float** outputs, uint32_t frames) override;

    // -------------------------------------------------------------------

private:
    Mutex fMutex;
    glBarsState* fState;
    float fParameters[kParameterCount];
    friend class DistrhoUIGLBars;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoPluginGLBars)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_GLBARS_HPP_INCLUDED
