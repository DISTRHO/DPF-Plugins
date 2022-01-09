/*
 * DISTRHO ProM Plugin
 * Copyright (C) 2015-2022 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * For a full copy of the license see the LICENSE file.
 */

/* s/private/public/ as workaround old projectM versions */
#define private public
#include "libprojectM/projectM.hpp"
#undef private

#ifndef PROJECTM_DATA_DIR
#include "libprojectM/projectM-opengl.h"
#endif

#include "DistrhoPluginProM.hpp"
#include "DistrhoUIProM.hpp"

#include "DistrhoPluginUtils.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

DistrhoUIProM::DistrhoUIProM()
    : UI(512, 512),
      fPM(nullptr),
      fResizeHandle(this)
{
    const double scaleFactor = getScaleFactor();

    if (d_isNotZero(scaleFactor))
        setSize(512*scaleFactor, 512*scaleFactor);

    setGeometryConstraints(256*scaleFactor, 256*scaleFactor, true);

    // no need to show resize handle if window is user-resizable
    // if (isResizable())
    //     fResizeHandle.hide();
}

DistrhoUIProM::~DistrhoUIProM()
{
    if (fPM == nullptr)
        return;

    if (DistrhoPluginProM* const dspPtr = (DistrhoPluginProM*)getPluginInstancePointer())
    {
        const MutexLocker csm(dspPtr->fMutex);
        dspPtr->fPM = nullptr;
    }
}

// -----------------------------------------------------------------------
// DSP Callbacks

void DistrhoUIProM::parameterChanged(uint32_t, float)
{
}

// -----------------------------------------------------------------------
// UI Callbacks

void DistrhoUIProM::uiIdle()
{
    if (fPM == nullptr)
        return;

    repaint();

    if (DistrhoPluginProM* const dspPtr = (DistrhoPluginProM*)getPluginInstancePointer())
    {
        if (dspPtr->fPM != nullptr)
            return;

        const MutexLocker csm(dspPtr->fMutex);
        dspPtr->fPM = fPM;
    }
}

void DistrhoUIProM::uiReshape(const uint width, const uint height)
{
    UI::uiReshape(width, height);

    if (fPM == nullptr)
    {
#ifdef PROJECTM_DATA_DIR
        fPM = new projectM(PROJECTM_DATA_DIR "/config.inp");
#else
        if (const char* const bundlePath = getBundlePath())
        {
            const String datadir(getResourcePath(bundlePath));
            d_stdout("ProM datadir: '%s'", datadir.buffer());

            projectM::Settings settings;
            settings.presetURL    = datadir + DISTRHO_OS_SEP_STR "presets";
            settings.titleFontURL = datadir + DISTRHO_OS_SEP_STR "fonts" DISTRHO_OS_SEP_STR "Vera.ttf";
            settings.menuFontURL  = datadir + DISTRHO_OS_SEP_STR "fonts" DISTRHO_OS_SEP_STR "VeraMono.ttf";
            settings.datadir      = datadir;
            fPM = new projectM(settings);
        }
        else
        {
            d_stderr2("ProM: failed to find bundle path, UI will be empty");
        }
#endif
    }

    if (fPM != nullptr)
        fPM->projectM_resetGL(width, height);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void DistrhoUIProM::onDisplay()
{
    if (fPM == nullptr)
        return;

    fPM->renderFrame();

    // some projectM versions do not turn off the last set GL program
    glUseProgram(0);
}

static projectMKeycode dgl2pmkey(const DGL_NAMESPACE::Key key) noexcept
{
    switch (key)
    {
    case DGL_NAMESPACE::kKeyBackspace:
        return PROJECTM_K_BACKSPACE;
    case DGL_NAMESPACE::kKeyEscape:
        return PROJECTM_K_ESCAPE;
    case DGL_NAMESPACE::kKeyDelete:
        return PROJECTM_K_DELETE;
    case DGL_NAMESPACE::kKeyF1:
        return PROJECTM_K_F1;
    case DGL_NAMESPACE::kKeyF2:
        return PROJECTM_K_F2;
    case DGL_NAMESPACE::kKeyF3:
        return PROJECTM_K_F3;
    case DGL_NAMESPACE::kKeyF4:
        return PROJECTM_K_F4;
    case DGL_NAMESPACE::kKeyF5:
        return PROJECTM_K_F5;
    case DGL_NAMESPACE::kKeyF6:
        return PROJECTM_K_F6;
    case DGL_NAMESPACE::kKeyF7:
        return PROJECTM_K_F7;
    case DGL_NAMESPACE::kKeyF8:
        return PROJECTM_K_F8;
    case DGL_NAMESPACE::kKeyF9:
        return PROJECTM_K_F9;
    case DGL_NAMESPACE::kKeyF10:
        return PROJECTM_K_F10;
    case DGL_NAMESPACE::kKeyF11:
        return PROJECTM_K_F11;
    case DGL_NAMESPACE::kKeyF12:
        return PROJECTM_K_F12;
    case DGL_NAMESPACE::kKeyLeft:
        return PROJECTM_K_LEFT;
    case DGL_NAMESPACE::kKeyUp:
        return PROJECTM_K_UP;
    case DGL_NAMESPACE::kKeyRight:
        return PROJECTM_K_RIGHT;
    case DGL_NAMESPACE::kKeyDown:
        return PROJECTM_K_DOWN;
    case DGL_NAMESPACE::kKeyPageUp:
        return PROJECTM_K_PAGEUP;
    case DGL_NAMESPACE::kKeyPageDown:
        return PROJECTM_K_PAGEDOWN;
    case DGL_NAMESPACE::kKeyHome:
        return PROJECTM_K_HOME;
    case DGL_NAMESPACE::kKeyEnd:
        return PROJECTM_K_END;
    case DGL_NAMESPACE::kKeyInsert:
        return PROJECTM_K_INSERT;
    case DGL_NAMESPACE::kKeyShiftL:
        return PROJECTM_K_LSHIFT;
    case DGL_NAMESPACE::kKeyShiftR:
        return PROJECTM_K_RSHIFT;
    case DGL_NAMESPACE::kKeyControlL:
        return PROJECTM_K_LCTRL;
    case DGL_NAMESPACE::kKeyControlR:
    case DGL_NAMESPACE::kKeyAltL:
    case DGL_NAMESPACE::kKeyAltR:
    case DGL_NAMESPACE::kKeySuperL:
    case DGL_NAMESPACE::kKeySuperR:
    case DGL_NAMESPACE::kKeyMenu:
    case DGL_NAMESPACE::kKeyCapsLock:
    case DGL_NAMESPACE::kKeyScrollLock:
    case DGL_NAMESPACE::kKeyNumLock:
    case DGL_NAMESPACE::kKeyPrintScreen:
    case DGL_NAMESPACE::kKeyPause:
        break;
    }

    return PROJECTM_K_NONE;
}

bool DistrhoUIProM::onKeyboard(const KeyboardEvent& ev)
{
    if (fPM == nullptr)
        return false;

#ifdef HAVE_PROJECTM_TEXT_FUNCTIONS
    // special handling for text
    if (fPM->isTextInputActive(true) && !ev.press)
    {
        if (ev.key >= ' ' && ev.key <= 'z')
        {
            std::string key;
            key += static_cast<char>(ev.key);
            fPM->setSearchText(key);
            return true;
        }
        else if (ev.key == DGL_NAMESPACE::kKeyBackspace)
        {
            fPM->deleteSearchText();
            return true;
        }
    }
#endif

    projectMKeycode pmKey = PROJECTM_K_NONE;

    if (ev.key >= kKeyF1)
    {
        pmKey = dgl2pmkey(static_cast<DGL_NAMESPACE::Key>(ev.key));
    }
    else if ((ev.key >= PROJECTM_K_0 && ev.key <= PROJECTM_K_9) ||
             (ev.key >= PROJECTM_K_A && ev.key <= PROJECTM_K_Z) ||
             (ev.key >= PROJECTM_K_a && ev.key <= PROJECTM_K_z))
    {
        pmKey = static_cast<projectMKeycode>(ev.key);

        if (ev.key >= PROJECTM_K_A && ev.key <= PROJECTM_K_Z && (ev.mod & DGL_NAMESPACE::kModifierShift))
            pmKey = static_cast<projectMKeycode>(pmKey + (PROJECTM_K_a - PROJECTM_K_A));
    }
    else
    {
        /* missing:
         * PROJECTM_K_CAPSLOCK
         */
        switch (ev.key)
        {
        case DGL_NAMESPACE::kKeyBackspace:
            pmKey = PROJECTM_K_BACKSPACE;
            break;
        case DGL_NAMESPACE::kKeyEscape:
            pmKey = PROJECTM_K_ESCAPE;
            break;
        case DGL_NAMESPACE::kKeyDelete:
            pmKey = PROJECTM_K_DELETE;
            break;
        case '\r':
            pmKey = PROJECTM_K_RETURN;
            break;
#ifdef HAVE_PROJECTM_TEXT_FUNCTIONS
        case '/':
            pmKey = PROJECTM_K_SLASH;
            break;
        case '\\':
            pmKey = PROJECTM_K_BACKSLASH;
            break;
#endif
        case '+':
            pmKey = PROJECTM_K_PLUS;
            break;
        case '-':
            pmKey = PROJECTM_K_MINUS;
            break;
        case '=':
            pmKey = PROJECTM_K_EQUALS;
            break;
        default:
            // d_stdout("Unhandled key %u %u %c", ev.keycode, ev.key, ev.key);
            break;
        }
    }

    if (pmKey == PROJECTM_K_NONE)
        return false;

    fPM->default_key_handler(ev.press ? PROJECTM_KEYUP : PROJECTM_KEYDOWN, pmKey);
    return true;
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUIProM();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
