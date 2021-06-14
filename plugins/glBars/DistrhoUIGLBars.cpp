/*
 * DISTRHO glBars Plugin based on XMMS/XBMC "GL Bars"
 * Copyright (C) 1998-2000  Peter Alm, Mikael Alm, Olle Hallnas, Thomas Nilsson and 4Front Technologies
 * Copyright (C) 2000 Christian Zander <phoenix@minion.de>
 * Copyright (C) 2015 Nedko Arnaudov
 * Copyright (C) 2016-2019 Filipe Coelho <falktx@falktx.com>
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
#include "DistrhoUIGLBars.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoUIGLBars::DistrhoUIGLBars()
    : UI(512, 512)
{
    setGeometryConstraints(256, 256, true);
}

DistrhoUIGLBars::~DistrhoUIGLBars()
{
    if (DistrhoPluginGLBars* const dspPtr = (DistrhoPluginGLBars*)getPluginInstancePointer())
    {
        const MutexLocker csm(dspPtr->fMutex);
        dspPtr->fState = nullptr;
    }
}

// -----------------------------------------------------------------------
// DSP Callbacks

void DistrhoUIGLBars::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case kParameterScale:
        fState.scale = value;
        break;

    case kParameterSpeed:
        fState.hSpeed = value;
        break;

    case kParameterX:
        fState.x_speed = value;
        break;

    case kParameterY:
        fState.y_speed = value;
        break;

    case kParameterZ:
        fState.z_speed = value;
        break;
    }
}

// -----------------------------------------------------------------------
// UI Callbacks

void DistrhoUIGLBars::uiIdle()
{
    repaint();

    if (DistrhoPluginGLBars* const dspPtr = (DistrhoPluginGLBars*)getPluginInstancePointer())
    {
        if (dspPtr->fState != nullptr)
            return;

        const MutexLocker csm(dspPtr->fMutex);
        dspPtr->fState = &fState;
    }
}

// -----------------------------------------------------------------------
// Widget Callbacks

void DistrhoUIGLBars::onDisplay()
{
    fState.Render();
}

bool DistrhoUIGLBars::onKeyboard(const KeyboardEvent& ev)
{
    if (ev.press && (ev.key == '1' || ev.key == '+' || ev.key == '-'))
    {
        if (ev.key == '1')
        {
            if (getWidth() != 512 || getHeight() != 512)
                setSize(512, 512);
        }
        else if (ev.key == '+')
        {
            /**/ if (getWidth() < 1100 && getHeight() < 1100)
                setSize(std::min(getWidth()+100, 1100U), std::min(getHeight()+100, 1100U));
            else if (getWidth() != 1100 || getHeight() != 1100)
                    setSize(1100, 1100);
        }
        else if (ev.key == '-')
        {
            /**/ if (getWidth() > 100 && getHeight() > 100)
                setSize(std::max(getWidth()-100, 100U), std::max(getHeight()-100, 100U));
            else if (getWidth() != 100 || getHeight() != 100)
                setSize(100, 100);
        }

        return true;
    }

    return true;
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUIGLBars();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
