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

#ifndef DISTRHO_UI_GLBARS_HPP_INCLUDED
#define DISTRHO_UI_GLBARS_HPP_INCLUDED

#include "DistrhoUI.hpp"

#include "glBars.hpp"
#include "ResizeHandle.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoUIGLBars : public UI
{
public:
    DistrhoUIGLBars();
    ~DistrhoUIGLBars() override;

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t, float) override;

    // -------------------------------------------------------------------
    // UI Callbacks

    void uiIdle() override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void onDisplay() override;

private:
    bool fInitialized;
    glBarsState fState;
    ResizeHandle fResizeHandle;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoUIGLBars)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // DISTRHO_UI_GLBARS_HPP_INCLUDED
