/*
 * DISTRHO AmplitudeImposer, a DPF'ied AmplitudeImposer.
 * Copyright (C) 2004 Niall Moody
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

#ifndef DISTRHO_UI_AMPLITUDE_IMPOSER_HPP_INCLUDED
#define DISTRHO_UI_AMPLITUDE_IMPOSER_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "ImageWidgets.hpp"

#include "DistrhoArtworkAmplitudeImposer.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class DistrhoUIAmplitudeImposer : public UI,
                                  public ImageSlider::Callback
{
public:
    DistrhoUIAmplitudeImposer();

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(uint32_t index, float value) override;
    void programLoaded(uint32_t index) override;

    // -------------------------------------------------------------------
    // Widget Callbacks

    void imageSliderDragStarted(ImageSlider* slider) override;
    void imageSliderDragFinished(ImageSlider* slider) override;
    void imageSliderValueChanged(ImageSlider* slider, float value) override;

    void onDisplay() override;

private:
    Image fImgBackground;
    ScopedPointer<ImageSlider> fSliderDepth, fSliderThres;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DistrhoUIAmplitudeImposer)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif // DISTRHO_UI_AMPLITUDE_IMPOSER_HPP_INCLUDED
