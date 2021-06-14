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

#include "DistrhoUIAmplitudeImposer.hpp"
#include "DistrhoPluginAmplitudeImposer.hpp"

START_NAMESPACE_DISTRHO

namespace Art = DistrhoArtworkAmplitudeImposer;

// -----------------------------------------------------------------------

DistrhoUIAmplitudeImposer::DistrhoUIAmplitudeImposer()
    : UI(Art::backWidth, Art::backHeight),
      fImgBackground(Art::backData, Art::backWidth, Art::backHeight, kImageFormatGrayscale)
{
    // sliders
    Image sliderImage(Art::sliderData, Art::sliderWidth, Art::sliderHeight, kImageFormatGrayscale);

    fSliderDepth = new ImageSlider(this, sliderImage);
    fSliderDepth->setId(DistrhoPluginAmplitudeImposer::kParameterDepth);
    fSliderDepth->setStartPos(108, 71);
    fSliderDepth->setEndPos(268, 71);
    fSliderDepth->setRange(0.0f, 1.0f);
    fSliderDepth->setCallback(this);

    fSliderThres = new ImageSlider(this, sliderImage);
    fSliderThres->setId(DistrhoPluginAmplitudeImposer::kParameterThreshold);
    fSliderThres->setStartPos(108, 110);
    fSliderThres->setEndPos(268, 110);
    fSliderThres->setRange(0.0f, 1.0f);
    fSliderThres->setCallback(this);

    // set initial values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void DistrhoUIAmplitudeImposer::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case DistrhoPluginAmplitudeImposer::kParameterDepth:
        fSliderDepth->setValue(value);
        break;
    case DistrhoPluginAmplitudeImposer::kParameterThreshold:
        fSliderThres->setValue(value);
        break;
    }
}

void DistrhoUIAmplitudeImposer::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    fSliderDepth->setValue(1.0f);
    fSliderThres->setValue(0.5f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void DistrhoUIAmplitudeImposer::imageSliderDragStarted(ImageSlider* slider)
{
    editParameter(slider->getId(), true);
}

void DistrhoUIAmplitudeImposer::imageSliderDragFinished(ImageSlider* slider)
{
    editParameter(slider->getId(), false);
}

void DistrhoUIAmplitudeImposer::imageSliderValueChanged(ImageSlider* slider, float value)
{
    setParameterValue(slider->getId(), value);
}

void DistrhoUIAmplitudeImposer::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());

    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUIAmplitudeImposer();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
