/*
  ==============================================================================

    TriggeredOscilloscope.cpp
    Created: 28 Feb 2023 4:24:56pm
    Author:  wadda

  ==============================================================================
*/

#include "TriggeredOscilloscope.h"

TriggeredOscilloscope::TriggeredOscilloscope(OscilloscopeAudioProcessor &aProcessor, int sampleRate)
    : OscilloscopeComponent(aProcessor, sampleRate, aProcessor.getEditorRefreshRate())
{
  
  float decayTimeRelative = aProcessor.getTreeState()->getParameterAsValue("decayTime").getValue();
  decayTimeRelative = std::max(static_cast<float>(0.05), decayTimeRelative);
  decayCounter.reset(
    new DecayCounter(
      aProcessor.getEditorRefreshRate(), 
      decayTimeRelative * OSCILLOSCOPE_MAX_DECAY_TIME()
    )
  );
  aProcessor.getTreeState()->addParameterListener("decayTime", decayCounter.get());

  refreshDislayed();
}

TriggeredOscilloscope::~TriggeredOscilloscope()
{
  audioProcessor.getTreeState()->removeParameterListener("decayTime", decayCounter.get());
}

void TriggeredOscilloscope::refreshDislayed()
{
  currentlyDisplayedData.resize(sampleData.size());
  std::copy(sampleData.begin(), sampleData.end(), currentlyDisplayedData.begin());
    
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();
  bool decrescentSlope = audioProcessor.getTreeState()->getParameterAsValue("slopeButtonTriggered").getValue();
  triggerLevel = audioProcessor.getTreeState()->getParameterAsValue("triggerLevel").getValue();
  firstSampleToPlot = currentlyDisplayedData.begin();

  auto max_el = *std::max_element(currentlyDisplayedData.begin(), currentlyDisplayedData.end());

  bool found = false;
  
  if (decrescentSlope)
  {
    // while not at trigger level or with correct slope
    auto condition = [this](float x, float subseq){
      bool firstCondition = std::abs(x - triggerLevel) < 0.05;
      bool secondCondition = subseq < x;
      return firstCondition && secondCondition;
    };
    for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i) {
      if (condition(i[0], i[1])) {
        firstSampleToPlot = i;
        found = true;
        break;
      }
    }
  }
  else
  {
    // while not at trigger level or with correct slope
    auto condition = [this](float x, float subseq){
      bool firstCondition = std::abs(x - triggerLevel) < 0.05;
      bool secondCondition = subseq > x;
      return firstCondition && secondCondition;
    };
    for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i) {
      if (condition(i[0], i[1])) {
        firstSampleToPlot = i;
        found = true;
        break;
      }
    }
  }
  if (!found)
  {
    std::fill(currentlyDisplayedData.begin(), currentlyDisplayedData.end(), 0.);
    firstSampleToPlot = currentlyDisplayedData.begin();
  }
}

std::vector<float>::iterator TriggeredOscilloscope::closest(std::vector<float>::iterator begin, std::vector<float>::iterator end, float value)
{
    auto closest = std::lower_bound(begin, end, value);
    return closest;
}

void TriggeredOscilloscope::plot(juce::Graphics &g, juce::Rectangle<float> rect, float scaler, float offset)
{
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();
  
  auto w = rect.getWidth();
  auto h = rect.getHeight();
  auto right = rect.getRight();
  auto center = rect.getBottom() - offset;
  auto gain = h * scaler;
  g.setColour(WAVEFORMCOLOUR());

  int sampleOffset = std::distance(data, firstSampleToPlot);
  float widthRatio = sampleOffset / numSamples;

  for (size_t i = 1; i < numSamples - sampleOffset; ++i)
    {
      auto xMapPrevious = juce::jmap(
        float(i - 1), 
        float(sampleOffset), 
        float(numSamples - 1), 
        float(right - w), 
        float(right * (1 - widthRatio))
      );
      auto xMap = juce::jmap(
        float(i), 
        float(sampleOffset), 
        float(numSamples - 1), 
        float(right - w), 
        float(right * (1 - widthRatio))
      );

      g.drawLine({ xMapPrevious,
                  center - gain * firstSampleToPlot[i - 1],
                  xMap,
                  center - gain * firstSampleToPlot[i] });
    }
}

void TriggeredOscilloscope::subclassSpecificCallback()
{
  if (decayCounter.get()->refreshNeeded())
  {
    refreshDislayed();
  }
}
