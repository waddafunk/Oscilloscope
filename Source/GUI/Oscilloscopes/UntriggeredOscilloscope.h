/*
  ==============================================================================

    UntriggeredOscilloscope.h
    Created: 28 Feb 2023 4:25:16pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include<JuceHeader.h>
#include"OscilloscopeComponent.h"
#include "ColorPalette.h"


/**
 * Untriggered oscilloscope. Displays data continuously updating with no control 
 * over trigger point and phase.
 */
class UntriggeredOscilloscope : public OscilloscopeComponent
{
  public:
  UntriggeredOscilloscope(OscilloscopeAudioProcessor& aProcessor, int sampleRate);

  private:
  void plot(
      juce::Graphics& g,
      juce::Rectangle<float> rect,
      float scaler = float(1),
      float offset = float(0)) override;

  void subclassSpecificCallback() override;

};
