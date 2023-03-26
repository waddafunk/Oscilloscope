/*
  ==============================================================================

    TriggerListener.h
    Created: 26 Mar 2023 2:06:54pm
    Author:  covariant

  ==============================================================================
*/

#pragma once
# include <JuceHeader.h>
#include "OscilloscopeComponent.h"
#include "UntriggeredOscilloscope.h"
#include "TriggeredOscilloscope.h"

/**
 * Class listening whether the oscilloscope is set on triggered or untriggered mode
 * and resetting the OscilloscopeComponent properly.
 * 
 */
class TriggerListener : public juce::AudioProcessorValueTreeState::Listener
{

public:
  TriggerListener(std::function<void()> setTriggered, std::function<void()> setUntriggered);

  /**
 * Callback executed each time isTriggered param of PluginProcessors' TreeState
 * is modified.
 * 
 * \param parameterID Parameter ID, always isTriggered
 * \param newValue Whether if the oscilloscope is triggered.
 */
  void parameterChanged (const juce::String& parameterID, float newValue) override;

private:
/**
 * Lambda to execute if Oscilloscope is being set to triggered mode.
 * 
 */
  std::function<void()> setTriggered;

/**
 * Lambda to execute if Oscilloscope is being set to untriggered mode.
 * 
 */
  std::function<void()> setUntriggered;
};


