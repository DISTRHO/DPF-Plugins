/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2022 Filipe Coelho <falktx@falktx.com>
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

#include "ApplicationPrivateData.hpp"

#if defined(__EMSCRIPTEN__)
# include <emscripten/emscripten.h>
#elif defined(DISTRHO_OS_MAC)
# include <CoreFoundation/CoreFoundation.h>
#endif

START_NAMESPACE_DGL

// --------------------------------------------------------------------------------------------------------------------

#ifdef __EMSCRIPTEN__
static void app_idle(void* const app)
{
    static_cast<Application*>(app)->idle();
}
#endif

Application::Application(const bool isStandalone)
    : pData(new PrivateData(isStandalone)) {}

Application::~Application()
{
    delete pData;
}

void Application::idle()
{
    pData->idle(0);
}

void Application::exec(const uint idleTimeInMs)
{
    DISTRHO_SAFE_ASSERT_RETURN(pData->isStandalone,);

#if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop_arg(app_idle, this, 0, true);
#elif defined(DISTRHO_OS_MAC)
    const CFTimeInterval idleTimeInSecs = static_cast<CFTimeInterval>(idleTimeInMs) / 1000;

    while (! pData->isQuitting)
    {
        pData->idle(0);

        if (CFRunLoopRunInMode(kCFRunLoopDefaultMode, idleTimeInSecs, true) == kCFRunLoopRunFinished)
            break;
    }
#else
    while (! pData->isQuitting)
        pData->idle(idleTimeInMs);
#endif
}

void Application::quit()
{
    pData->quit();
}

bool Application::isQuitting() const noexcept
{
    return pData->isQuitting || pData->isQuittingInNextCycle;
}

bool Application::isStandalone() const noexcept
{
    return pData->isStandalone;
}

double Application::getTime() const
{
    return pData->getTime();
}

void Application::addIdleCallback(IdleCallback* const callback)
{
    DISTRHO_SAFE_ASSERT_RETURN(callback != nullptr,)

    pData->idleCallbacks.push_back(callback);
}

void Application::removeIdleCallback(IdleCallback* const callback)
{
    DISTRHO_SAFE_ASSERT_RETURN(callback != nullptr,)

    pData->idleCallbacks.remove(callback);
}

void Application::setClassName(const char* const name)
{
    pData->setClassName(name);
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DGL
