/*
  ==============================================================================

    BasicControls.cpp
    Created: 16 Mar 2023 11:52:22am
    Author:  covariant

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BasicControls.h"

//==============================================================================
BasicControls::BasicControls()
{
  // add and make visible elements
  addAndMakeVisible(drawGrid);
  addAndMakeVisible(bufferLength);
  addAndMakeVisible(muteOutput);

  // set texts
  drawGrid.setButtonText("Grid");
  muteOutput.setButtonText("Mute");

  // set styles
  bufferLength.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
  bufferLength.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
}

BasicControls::~BasicControls()
{
}

void BasicControls::paint(juce::Graphics &g)
{
}

void BasicControls::resized()
{
  /**
   * Set sizes and positions for each element.
   *
   */

  drawGrid.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  drawGrid.setTopLeftPosition(10, getHeight() / 8.);

  muteOutput.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  muteOutput.setTopLeftPosition(10 + getWidth() / 5, getHeight() / 8.);

  bufferLength.setSize(getWidth() * 2 / 5, getHeight() * 3. / 4.);
  bufferLength.setTopLeftPosition(getWidth() * 11. / 20., getHeight() / 8.);
}
