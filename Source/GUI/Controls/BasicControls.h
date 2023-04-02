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
class BasicControls : public juce::Component
{
public:
  /**
   * Construct a new Basic Controls object.
   *
   */
  BasicControls();

  /**
   * Destroy the Basic Controls object.
   *
   */
  ~BasicControls() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  /**
   * Controls whether to draw the grid.
   *
   */
  juce::ToggleButton drawGrid;

  /**
   * Controls whether to mute the output.
   *
   */
  juce::ToggleButton muteOutput;

  /**
   * Controls length displayed.
   *
   */
  juce::Slider bufferLength;

  /**
   * bufferLength attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bufferLengthAttachment;

  /**
   * drawGrid attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gridAttachment;

  /**
   * muteOutput attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteOutputAttachment;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BasicControls)
};
