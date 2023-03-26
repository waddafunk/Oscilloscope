/*
  ==============================================================================

    DecayCounter.h
    Created: 23 Mar 2023 8:10:55pm
    Author:  covariant

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "InitVariables.h"

/**
 * Class keeping track of decay time of the Oscilloscope
 * 
 */
class DecayCounter : public juce::AudioProcessorValueTreeState::Listener
{
private:
  
  /**
   * Decay time. Represents the number of frames to wait before
   * refreshing the wave plot.
   * 
   */
  int decayTime;

  /**
   * Maximum decay time in seconds.
   * 
   */
  int maxDecayTime;

  /**
   * Editor's frames per second.
   * 
   */
  int framesPerSecond;

  /**
   * Decay time in seconds.
   * 
   */
  int decaySeconds;

public:
  /**
   * Construct a new Decay Counter object
   * 
   * \param framesPerSecond PluginEditor refresh rate.
   * \param decaySeconds Decay time in seconds.
   */
  DecayCounter(int framesPerSecond, int decaySeconds);
  ~DecayCounter();

  /**
   * Decrements decay time and rturns whether if it is time to 
   * refresh the wave plot.
   * 
   * \return true if decayTime <= 0.
   * \return false otherwise.
   */
  bool refreshNeeded();

  /**
   * Resets decay time to framesPerSeconds * decaySeconds
   * 
   */
  void resetDecayTime();

  /**
   * Listens to changes in AudioProcessorValueTreeState's  "decayTime"
   * and updates decayTime accordingly
   * 
   * \param parameterID Always "decayTime".
   * \param newValue New value (between 0 and 1).
   */
  void parameterChanged (const juce::String& parameterID, float newValue) override;
};


