/*
  ==============================================================================

    BasicControls.h
    Created: 16 Mar 2023 11:52:22am
    Author:  covariant

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BasicControls  : public juce::Component
{
public:
  BasicControls();
  ~BasicControls() override;
  
  void paint (juce::Graphics&) override;
  void resized() override;

  juce::ToggleButton drawGrid;
  juce::ToggleButton muteOutput;
  juce::Slider bufferLength;
  std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> bufferLengthAttachment;
  std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> gridAttachment;
  std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> muteOutputAttachment;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicControls)
};
