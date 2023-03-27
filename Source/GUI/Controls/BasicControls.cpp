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

  addAndMakeVisible(drawGrid);
  addAndMakeVisible(bufferLength);
  addAndMakeVisible(muteOutput);

  drawGrid.setButtonText("Grid");
  muteOutput.setButtonText("Mute");

  bufferLength.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
  bufferLength.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

}

BasicControls::~BasicControls()
{
}

void BasicControls::paint (juce::Graphics& g)
{
}

void BasicControls::resized()
{
  
  drawGrid.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  drawGrid.setTopLeftPosition(10, getHeight() / 8.);

  muteOutput.setSize(getWidth() / 5., getHeight() * 3. / 4.);
  muteOutput.setTopLeftPosition(10 + getWidth() / 5, getHeight() / 8.);

  bufferLength.setSize(getWidth() * 2 / 5, getHeight() * 3. / 4.);
  bufferLength.setTopLeftPosition(getWidth() * 11. / 20., getHeight() / 8.);

}
