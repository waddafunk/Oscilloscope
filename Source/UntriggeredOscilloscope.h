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

class UntriggeredOscilloscope : public OscilloscopeComponent
{
public:
UntriggeredOscilloscope(OscilloscopeAudioProcessor& aProcessor, int sampleRate);

void plot(const float* data,
    size_t numSamples,
    juce::Graphics& g,
    juce::Rectangle<float> rect,
    float scaler = float(1),
    float offset = float(0)) override;
};
