/*
  ==============================================================================

    TriggeredOscilloscope.h
    Created: 28 Feb 2023 4:24:56pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include "OscilloscopeComponent.h"
#include "DecayCounter.h"
#include "InitVariables.h"
#include "ColorPalette.h"

class TriggeredOscilloscope : private OscilloscopeComponent
{

public:

  /**
   * Construct a new Triggered Oscilloscope object
   * 
   * \param aProcessor PluginProcessor audio processor.
   * \param sampleRate Sample rate of the host.
   */
  TriggeredOscilloscope(OscilloscopeAudioProcessor& aProcessor, int sampleRate);
  ~TriggeredOscilloscope();

private:

  /**
   * Class keeping track of decay time.
   * 
   */
  DecayCounter decayCounter;

  /**
   * Data currently displayed.
   * 
   */
  std::vector<float> currentlyDisplayedData;
  
  void plot(
      juce::Graphics& g,
      juce::Rectangle<float> rect,
      float scaler = float(1),
      float offset = float(0)) override;

  void subclassSpecificCallback() override;

};

