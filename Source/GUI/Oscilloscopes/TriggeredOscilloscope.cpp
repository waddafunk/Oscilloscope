/*
  ==============================================================================

    TriggeredOscilloscope.cpp
    Created: 28 Feb 2023 4:24:56pm
    Author:  wadda

  ==============================================================================
*/

#include "TriggeredOscilloscope.h"

TriggeredOscilloscope::TriggeredOscilloscope(OscilloscopeAudioProcessor &aProcessor, int sampleRate)
    : OscilloscopeComponent(aProcessor, sampleRate, aProcessor.getEditorRefreshRate()),
    decayCounter(
      aProcessor.getEditorRefreshRate(), 
      float(aProcessor.getTreeState()->getParameterAsValue("decayTime").getValue()) * OSCILLOSCOPE_MAX_DECAY_TIME()
    )
{
  aProcessor.getTreeState()->addParameterListener("decayTime", &decayCounter);
  currentlyDisplayedData = std::vector<float>(this->sampleData);
}

TriggeredOscilloscope::~TriggeredOscilloscope()
{
}

void TriggeredOscilloscope::plot(juce::Graphics &g, juce::Rectangle<float> rect, float scaler, float offset)
{
  auto data = currentlyDisplayedData.data();
  auto numSamples = currentlyDisplayedData.size();
  
  auto w = rect.getWidth();
  auto h = rect.getHeight();
  auto right = rect.getRight();
  auto center = rect.getBottom() - offset;
  auto gain = h * scaler;
  g.setColour(WAVEFORMCOLOUR());

  bool decrescentSlope = audioProcessor.getTreeState()->getParameterAsValue("decayTime").getValue();
  float triggerLevel = audioProcessor.getTreeState()->getParameterAsValue("triggerLevel").getValue();
  float precision = 0.05;

  auto firstSampleToPlot = data;
  
  if (decrescentSlope)
  {
    // while not at trigger level or with correct slope
    while( 
      std::abs(*firstSampleToPlot - triggerLevel) > precision ||
      *(firstSampleToPlot + 1) > *firstSampleToPlot
      )
      {
        firstSampleToPlot++;
      }

  }
  else
  {
    // while not at trigger level or with correct slope
    while( 
      std::abs(*firstSampleToPlot - triggerLevel) > precision ||
      *(firstSampleToPlot + 1) < *firstSampleToPlot
      )
      {
        firstSampleToPlot++;
      }
  }

  int sampleOffset = firstSampleToPlot - data;
  float widthRatio = sampleOffset / numSamples;

  for (size_t i = 1; i < numSamples - sampleOffset; ++i)
    {
        g.drawLine({ juce::jmap(float(i - 1), float(sampleOffset), float(numSamples - 1), float(right - w), float(right * (1 - widthRatio))),
                      center - gain * firstSampleToPlot[i - 1],
                      juce::jmap(float(i), float(sampleOffset), float(numSamples - 1), float(right - w), float(right * (1 - widthRatio))),
                      center - gain * firstSampleToPlot[i] });
    }
}

void TriggeredOscilloscope::subclassSpecificCallback()
{
  if (decayCounter.refreshNeeded())
  {
    currentlyDisplayedData = std::vector<float>(this->sampleData);
  }
}
