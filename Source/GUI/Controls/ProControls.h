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
class ProControls  : public juce::Component
{
public:
    ProControls();
    ~ProControls() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::ToggleButton triggerButton;
    juce::ToggleButton slopeButton;
    juce::Slider triggerLevel;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> triggerLevelAttachment;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> triggerButtonAttachment;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> slopeButtonAttachment;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProControls)
};
