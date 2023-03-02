/*
  ==============================================================================

    UntriggeredOscilloscope.cpp
    Created: 28 Feb 2023 4:25:16pm
    Author:  wadda

  ==============================================================================
*/

#include "UntriggeredOscilloscope.h"

UntriggeredOscilloscope::UntriggeredOscilloscope(OscilloscopeAudioProcessor& aProcessor, int sampleRate)
    :OscilloscopeComponent(aProcessor, sampleRate, aProcessor.getEditorRefreshRate())
{
}

void UntriggeredOscilloscope::plot(const float* data, size_t numSamples, juce::Graphics& g, juce::Rectangle<float> rect, float scaler, float offset)
{
    auto w = rect.getWidth();
    auto h = rect.getHeight();
    auto right = rect.getRight();

    auto center = rect.getBottom() - offset;
    auto gain = h * scaler;

    g.setColour(juce::Colours::beige);

    for (size_t i = 1; i < numSamples; ++i)
        g.drawLine({ juce::jmap(float(i - 1), float(0), float(numSamples - 1), float(right - w), float(right)),
                      center - gain * data[i - 1],
                      juce::jmap(float(i), float(0), float(numSamples - 1), float(right - w), float(right)),
                      center - gain * data[i] });
}
