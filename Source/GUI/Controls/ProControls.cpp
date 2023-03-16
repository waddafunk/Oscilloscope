/*
  ==============================================================================

    ProControls.cpp
    Created: 16 Mar 2023 11:53:07am
    Author:  covariant

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ProControls.h"

//==============================================================================
ProControls::ProControls()
{
    addAndMakeVisible(triggerButton);
    addAndMakeVisible(slopeButton);
    addAndMakeVisible(triggerLevel);

    triggerButton.setButtonText("Trigger");
    slopeButton.setButtonText("Slope");

    triggerLevel.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    triggerLevel.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

}

ProControls::~ProControls()
{
}

void ProControls::paint (juce::Graphics& g)
{
}

void ProControls::resized()
{
  
  triggerButton.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  triggerButton.setTopLeftPosition(10, getHeight() / 8.);

  slopeButton.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  slopeButton.setTopLeftPosition(10 + getWidth() * 2. / 5., getHeight() / 8.);

  triggerLevel.setSize(getWidth() / 4, getHeight() * 3. / 4.);
  triggerLevel.setTopLeftPosition(getWidth() * 3. / 5., getHeight() / 8.);

}
