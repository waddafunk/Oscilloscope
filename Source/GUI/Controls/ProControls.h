/*
  ==============================================================================

    ProControls.h
    Created: 16 Mar 2023 11:53:07am
    Author:  covariant

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class ProControls : public juce::Component
{
public:
  /**
   * Construct a new Pro Controls object
   *
   */
  ProControls();
  
  /**
   * Destroy the Pro Controls object
   *
   */
  ~ProControls() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  /**
   * Button determining whether to show continuosly or statically data.
   *
   */
  juce::ToggleButton triggerButton;

  /**
   * Button to automatically trigger oscilloscope (automatically finds a suitable trigger level).
   *
   */
  juce::ToggleButton autoTriggerButton;

  /**
   * Button determining the slope from where to start plotting (crescent or decrescent)
   *
   */
  juce::ToggleButton slopeButton;

  /**
   * Slider determining the level from where to start plotting.
   *
   */
  juce::Slider triggerLevel;

  /**
   * Slider determining the time amount needed to refresh the view
   *
   */
  juce::Slider refreshTime;

  /**
   * @ref triggerLevel attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> triggerLevelAttachment;

  /**
   * @ref refreshTime attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> refreshTimeAttachment;

  /**
   * @ref triggerButton attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> triggerButtonAttachment;

  /**
   * @ref autoTriggerButton attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoTriggerButtonAttachment;

  /**
   * @ref slopeButton attachment.
   *
   */
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> slopeButtonAttachment;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProControls)
};
