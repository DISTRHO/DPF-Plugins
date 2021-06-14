/*
 * DISTRHO CycleShifter, a DPF'ied CycleShifter.
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

#include "DistrhoUICycleShifter.hpp"
#include "DistrhoPluginCycleShifter.hpp"

START_NAMESPACE_DISTRHO

namespace Art = DistrhoArtworkCycleShifter;

// -----------------------------------------------------------------------

DistrhoUICycleShifter::DistrhoUICycleShifter()
    : UI(Art::backWidth, Art::backHeight),
      fImgBackground(Art::backData, Art::backWidth, Art::backHeight, kImageFormatGrayscale)
{
    // sliders
    Image sliderImage(Art::sliderData, Art::sliderWidth, Art::sliderHeight, kImageFormatGrayscale);

    fSliderNewCycleVol = new ImageSlider(this, sliderImage);
    fSliderNewCycleVol->setId(DistrhoPluginCycleShifter::kParameterNewCycleVolume);
    fSliderNewCycleVol->setStartPos(6, 49);
    fSliderNewCycleVol->setEndPos(247, 49);
    fSliderNewCycleVol->setRange(0.0f, 1.0f);
    fSliderNewCycleVol->setCallback(this);

    fSliderInputVol = new ImageSlider(this, sliderImage);
    fSliderInputVol->setId(DistrhoPluginCycleShifter::kParameterInputVolume);
    fSliderInputVol->setStartPos(6, 80);
    fSliderInputVol->setEndPos(247, 80);
    fSliderInputVol->setRange(0.0f, 1.0f);
    fSliderInputVol->setCallback(this);

    // set initial values
    programLoaded(0);
}

// -----------------------------------------------------------------------
// DSP Callbacks

void DistrhoUICycleShifter::parameterChanged(uint32_t index, float value)
{
    switch (index)
    {
    case DistrhoPluginCycleShifter::kParameterNewCycleVolume:
        fSliderNewCycleVol->setValue(value);
        break;
    case DistrhoPluginCycleShifter::kParameterInputVolume:
        fSliderInputVol->setValue(value);
        break;
    }
}

void DistrhoUICycleShifter::programLoaded(uint32_t index)
{
    if (index != 0)
        return;

    fSliderNewCycleVol->setValue(1.0f);
    fSliderInputVol->setValue(1.0f);
}

// -----------------------------------------------------------------------
// Widget Callbacks

void DistrhoUICycleShifter::imageSliderDragStarted(ImageSlider* slider)
{
    editParameter(slider->getId(), true);
}

void DistrhoUICycleShifter::imageSliderDragFinished(ImageSlider* slider)
{
    editParameter(slider->getId(), false);
}

void DistrhoUICycleShifter::imageSliderValueChanged(ImageSlider* slider, float value)
{
    setParameterValue(slider->getId(), value);
}

void DistrhoUICycleShifter::onDisplay()
{
    const GraphicsContext& context(getGraphicsContext());

    fImgBackground.draw(context);
}

// -----------------------------------------------------------------------

UI* createUI()
{
    return new DistrhoUICycleShifter();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
