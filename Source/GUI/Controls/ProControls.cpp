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
    // make elements visible
    addAndMakeVisible(triggerButton);
    addAndMakeVisible(slopeButton);
    addAndMakeVisible(triggerLevel);
    addAndMakeVisible(refreshTime);

    // add autoTrigger
    addChildComponent(autoTriggerButton);

    // set buttons' text
    triggerButton.setButtonText("Trigger");
    autoTriggerButton.setButtonText("Auto");
    slopeButton.setButtonText("Slope");

    // show autoTrigger only if triggered
    triggerButton.onClick = [this](){
      bool isTriggered = triggerButton.getToggleState();
      autoTriggerButton.setVisible(isTriggered);
    };

    // set sliders style
    triggerLevel.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    triggerLevel.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    refreshTime.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    refreshTime.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

}

ProControls::~ProControls()
{
}

void ProControls::paint (juce::Graphics& g)
{
}

void ProControls::resized()
{
  /**
   * Set size and top left position for each element
   * 
   */

  triggerButton.setSize(getWidth() / 10., getHeight() * 3. / 4.);
  triggerButton.setTopLeftPosition(10, getHeight() / 8.);

  autoTriggerButton.setSize(getWidth() / 10., getHeight() * 3. / 4.);
  autoTriggerButton.setTopLeftPosition(10 + getWidth() / 10, getHeight() / 8.);

  slopeButton.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  slopeButton.setTopLeftPosition(10 + getWidth() * 2. / 5., getHeight() / 8.);

  triggerLevel.setSize(getWidth() / 8, getHeight() * 3. / 4.);
  triggerLevel.setTopLeftPosition(getWidth() * 3. / 5., getHeight() / 8.);

  refreshTime.setSize(getWidth() / 8, getHeight() * 3. / 4.);
  refreshTime.setTopLeftPosition(getWidth() * 7. / 10., getHeight() / 8.);

}
