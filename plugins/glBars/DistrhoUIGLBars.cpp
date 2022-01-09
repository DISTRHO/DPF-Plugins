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

#include "DistrhoPluginGLBars.hpp"
#include "DistrhoUIGLBars.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoUIGLBars::DistrhoUIGLBars()
    : UI(512, 512),
      fInitialized(false),
      fResizeHandle(this)
{
    const double scaleFactor = getScaleFactor();

    if (d_isNotZero(scaleFactor))
        setSize(512*scaleFactor, 512*scaleFactor);

    setGeometryConstraints(256*scaleFactor, 256*scaleFactor, true);

    // no need to show resize handle if window is user-resizable
    if (isResizable())
        fResizeHandle.hide();
}

DistrhoUIGLBars::~DistrhoUIGLBars()
{
    if (! fInitialized)
        return;

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

        fInitialized = true;

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

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUIGLBars();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
