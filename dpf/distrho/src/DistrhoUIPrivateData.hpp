/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2021 Filipe Coelho <falktx@falktx.com>
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

#ifndef DISTRHO_UI_PRIVATE_DATA_HPP_INCLUDED
#define DISTRHO_UI_PRIVATE_DATA_HPP_INCLUDED

#include "../DistrhoUI.hpp"
#include "../../dgl/Application.hpp"

#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI
# include "../../dgl/src/WindowPrivateData.hpp"
# include "../../dgl/src/pugl.hpp"
#endif

#if defined(DISTRHO_PLUGIN_TARGET_JACK) || defined(DISTRHO_PLUGIN_TARGET_DSSI)
# define DISTRHO_UI_IS_STANDALONE 1
#else
# define DISTRHO_UI_IS_STANDALONE 0
#endif

#if defined(DISTRHO_PLUGIN_TARGET_VST2)
# undef DISTRHO_UI_USER_RESIZABLE
# define DISTRHO_UI_USER_RESIZABLE 0
#endif

// -----------------------------------------------------------------------

START_NAMESPACE_DGL

// -----------------------------------------------------------------------
// Plugin Application, will set class name based on plugin details

class PluginApplication : public Application
{
public:
    explicit PluginApplication()
        : Application(DISTRHO_UI_IS_STANDALONE)
    {
        const char* const className = (
#ifdef DISTRHO_PLUGIN_BRAND
            DISTRHO_PLUGIN_BRAND
#else
            DISTRHO_MACRO_AS_STRING(DISTRHO_NAMESPACE)
#endif
            "-" DISTRHO_PLUGIN_NAME
        );
        setClassName(className);
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginApplication)
};

// -----------------------------------------------------------------------
// Plugin Window, will pass some Window events to UI

#if DISTRHO_PLUGIN_HAS_EXTERNAL_UI
// TODO external ui stuff
class PluginWindow
{
    DISTRHO_NAMESPACE::UI* const ui;

public:
    explicit PluginWindow(DISTRHO_NAMESPACE::UI* const uiPtr,
                          PluginApplication& app,
                          const uintptr_t parentWindowHandle,
                          const uint width,
                          const uint height,
                          const double scaleFactor)
        : Window(app, parentWindowHandle, width, height, scaleFactor, DISTRHO_UI_USER_RESIZABLE),
          ui(uiPtr) {}

    uint getWidth() const noexcept
    {
        return ui->getWidth();
    }

    uint getHeight() const noexcept
    {
        return ui->getHeight();
    }

    bool isVisible() const noexcept
    {
        return ui->isRunning();
    }

    uintptr_t getNativeWindowHandle() const noexcept
    {
        return 0;
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWindow)
};
#else
class PluginWindow : public Window
{
    DISTRHO_NAMESPACE::UI* const ui;

public:
    explicit PluginWindow(DISTRHO_NAMESPACE::UI* const uiPtr,
                          PluginApplication& app,
                          const uintptr_t parentWindowHandle,
                          const uint width,
                          const uint height,
                          const double scaleFactor)
        : Window(app, parentWindowHandle, width, height, scaleFactor, DISTRHO_UI_USER_RESIZABLE),
          ui(uiPtr)
    {
        puglBackendEnter(pData->view);
    }

    void leaveContext()
    {
        puglBackendLeave(pData->view);
    }

protected:
    void onFocus(const bool focus, const DGL_NAMESPACE::CrossingMode mode) override
    {
        DISTRHO_SAFE_ASSERT_RETURN(ui != nullptr,);

        ui->uiFocus(focus, mode);
    }

    void onReshape(const uint width, const uint height) override
    {
        DISTRHO_SAFE_ASSERT_RETURN(ui != nullptr,);

        ui->uiReshape(width, height);
    }

    void onScaleFactorChanged(const double scaleFactor) override
    {
        DISTRHO_SAFE_ASSERT_RETURN(ui != nullptr,);

        ui->uiScaleFactorChanged(scaleFactor);
    }

# ifndef DGL_FILE_BROWSER_DISABLED
    void onFileSelected(const char* filename) override;
# endif

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginWindow)
};
#endif // !DISTRHO_PLUGIN_HAS_EXTERNAL_UI

END_NAMESPACE_DGL

// -----------------------------------------------------------------------

START_NAMESPACE_DISTRHO

using DGL_NAMESPACE::PluginApplication;
using DGL_NAMESPACE::PluginWindow;

// -----------------------------------------------------------------------
// UI callbacks

typedef void (*editParamFunc)   (void* ptr, uint32_t rindex, bool started);
typedef void (*setParamFunc)    (void* ptr, uint32_t rindex, float value);
typedef void (*setStateFunc)    (void* ptr, const char* key, const char* value);
typedef void (*sendNoteFunc)    (void* ptr, uint8_t channel, uint8_t note, uint8_t velo);
typedef void (*setSizeFunc)     (void* ptr, uint width, uint height);
typedef bool (*fileRequestFunc) (void* ptr, const char* key);

// -----------------------------------------------------------------------
// UI private data

struct UI::PrivateData {
    // DGL
    PluginApplication app;
    ScopedPointer<PluginWindow> window;

    // DSP
    double   sampleRate;
    uint32_t parameterOffset;
    void*    dspPtr;

    // UI
    uint bgColor;
    uint fgColor;
#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI
    double scaleFactor;
    uintptr_t winId;
# ifndef DGL_FILE_BROWSER_DISABLED
    char* uiStateFileKeyRequest;
# endif
#endif

    // Callbacks
    void*           callbacksPtr;
    editParamFunc   editParamCallbackFunc;
    setParamFunc    setParamCallbackFunc;
    setStateFunc    setStateCallbackFunc;
    sendNoteFunc    sendNoteCallbackFunc;
    setSizeFunc     setSizeCallbackFunc;
    fileRequestFunc fileRequestCallbackFunc;

    PrivateData() noexcept
        : app(),
#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI
          window(nullptr),
#endif
          sampleRate(0),
          parameterOffset(0),
          dspPtr(nullptr),
          bgColor(0),
          fgColor(0xffffffff),
#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI
          scaleFactor(1.0),
          winId(0),
# ifndef DGL_FILE_BROWSER_DISABLED
          uiStateFileKeyRequest(nullptr),
# endif
#endif
          callbacksPtr(nullptr),
          editParamCallbackFunc(nullptr),
          setParamCallbackFunc(nullptr),
          setStateCallbackFunc(nullptr),
          sendNoteCallbackFunc(nullptr),
          setSizeCallbackFunc(nullptr),
          fileRequestCallbackFunc(nullptr)
    {
#if defined(DISTRHO_PLUGIN_TARGET_DSSI) || defined(DISTRHO_PLUGIN_TARGET_LV2)
        parameterOffset += DISTRHO_PLUGIN_NUM_INPUTS + DISTRHO_PLUGIN_NUM_OUTPUTS;
# if DISTRHO_PLUGIN_WANT_LATENCY
        parameterOffset += 1;
# endif
#endif

#ifdef DISTRHO_PLUGIN_TARGET_LV2
# if (DISTRHO_PLUGIN_WANT_MIDI_INPUT || DISTRHO_PLUGIN_WANT_TIMEPOS || DISTRHO_PLUGIN_WANT_STATE)
        parameterOffset += 1;
# endif
# if (DISTRHO_PLUGIN_WANT_MIDI_OUTPUT || DISTRHO_PLUGIN_WANT_STATE)
        parameterOffset += 1;
# endif
#endif
    }

    ~PrivateData() noexcept
    {
#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI && !defined(DGL_FILE_BROWSER_DISABLED)
        std::free(uiStateFileKeyRequest);
#endif
    }

    void editParamCallback(const uint32_t rindex, const bool started)
    {
        if (editParamCallbackFunc != nullptr)
            editParamCallbackFunc(callbacksPtr, rindex, started);
    }

    void setParamCallback(const uint32_t rindex, const float value)
    {
        if (setParamCallbackFunc != nullptr)
            setParamCallbackFunc(callbacksPtr, rindex, value);
    }

    void setStateCallback(const char* const key, const char* const value)
    {
        if (setStateCallbackFunc != nullptr)
            setStateCallbackFunc(callbacksPtr, key, value);
    }

    void sendNoteCallback(const uint8_t channel, const uint8_t note, const uint8_t velocity)
    {
        if (sendNoteCallbackFunc != nullptr)
            sendNoteCallbackFunc(callbacksPtr, channel, note, velocity);
    }

    void setSizeCallback(const uint width, const uint height)
    {
        if (setSizeCallbackFunc != nullptr)
            setSizeCallbackFunc(callbacksPtr, width, height);
    }

    // implemented below, after PluginWindow
    bool fileRequestCallback(const char* const key);

    static UI::PrivateData* s_nextPrivateData;
    static PluginWindow& createNextWindow(UI* ui, uint width, uint height);
};

// -----------------------------------------------------------------------
// UI private data fileRequestCallback, which requires PluginWindow definitions

inline bool UI::PrivateData::fileRequestCallback(const char* const key)
{
    if (fileRequestCallbackFunc != nullptr)
        return fileRequestCallbackFunc(callbacksPtr, key);

#if DISTRHO_PLUGIN_WANT_STATEFILES && !DISTRHO_PLUGIN_HAS_EXTERNAL_UI && !defined(DGL_FILE_BROWSER_DISABLED)
    std::free(uiStateFileKeyRequest);
    uiStateFileKeyRequest = strdup(key);
    DISTRHO_SAFE_ASSERT_RETURN(uiStateFileKeyRequest != nullptr, false);

    char title[0xff];
    snprintf(title, sizeof(title)-1u, DISTRHO_PLUGIN_NAME ": %s", key);
    title[sizeof(title)-1u] = '\0';

    DGL_NAMESPACE::Window::FileBrowserOptions opts;
    opts.title = title;
    return window->openFileBrowser(opts);
#endif

    return false;
}

END_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------
// PluginWindow onFileSelected that require UI::PrivateData definitions

#if !DISTRHO_PLUGIN_HAS_EXTERNAL_UI && !defined(DGL_FILE_BROWSER_DISABLED)
START_NAMESPACE_DGL

inline void PluginWindow::onFileSelected(const char* const filename)
{
    DISTRHO_SAFE_ASSERT_RETURN(ui != nullptr,);

# if DISTRHO_PLUGIN_WANT_STATEFILES
    if (char* const key = ui->uiData->uiStateFileKeyRequest)
    {
        ui->uiData->uiStateFileKeyRequest = nullptr;
        if (filename != nullptr)
        {
            // notify DSP
            ui->setState(key, filename);
            // notify UI
            ui->stateChanged(key, filename);
        }
        std::free(key);
        return;
    }
# endif

    ui->uiFileBrowserSelected(filename);
}

END_NAMESPACE_DGL
#endif

// -----------------------------------------------------------------------

#endif // DISTRHO_UI_PRIVATE_DATA_HPP_INCLUDED
