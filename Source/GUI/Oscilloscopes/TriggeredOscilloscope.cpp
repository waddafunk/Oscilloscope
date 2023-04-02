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
  // get decay time
  float decayTimeRelative = aProcessor.getTreeState()->getParameterAsValue("decayTime").getValue();
  decayTimeRelative = std::max(static_cast<float>(0.05), decayTimeRelative); // sometimes it inizializes to 0

  // reset decayCounter & add it as listener
  decayCounter.reset(
      new DecayCounter(
          aProcessor.getEditorRefreshRate(),
          decayTimeRelative * OSCILLOSCOPE_MAX_DECAY_TIME()));
  aProcessor.getTreeState()->addParameterListener("decayTime", decayCounter.get());

  // refresh displayed data
  refreshDislayed();
}

TriggeredOscilloscope::~TriggeredOscilloscope()
{
  // remove decayCounter as listener
  audioProcessor.getTreeState()->removeParameterListener("decayTime", decayCounter.get());
}

void TriggeredOscilloscope::refreshDislayed()
{
  // resize buffer and copy from sampleData
  currentlyDisplayedData.resize(sampleData.size());
  std::copy(sampleData.begin(), sampleData.end(), currentlyDisplayedData.begin());

  // declare variables
  bool found = false;
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();
  bool decrescentSlope = audioProcessor.getTreeState()->getParameterAsValue("slopeButtonTriggered").getValue();

  // get trigger level
  triggerLevel = audioProcessor.getTreeState()->getParameterAsValue("triggerLevel").getValue();

  // reset firstSampleToPlot
  firstSampleToPlot = currentlyDisplayedData.begin();

  // if decrescent slope search for trigger position with decrescent slope
  if (decrescentSlope)
  {
    // condition to stop searching (lamda func)
    auto condition = [this](float x, float subseq)
    {
      bool firstCondition = std::abs(x - triggerLevel) < 0.05;
      bool secondCondition = subseq < x;
      return firstCondition && secondCondition;
    };

    // search in all currentlyDisplayedData
    for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
    {
      // when found set firstSampleToPlot and stop
      if (condition(i[0], i[1]))
      {
        firstSampleToPlot = i;
        found = true;
        break;
      }
    }
  }
  // else search for trigger position with crescent slope
  else
  {
    // condition to stop searching (lamda func)
    auto condition = [this](float x, float subseq)
    {
      bool firstCondition = std::abs(x - triggerLevel) < 0.05;
      bool secondCondition = subseq > x;
      return firstCondition && secondCondition;
    };

    // search in all currentlyDisplayedData
    for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
    {
      firstSampleToPlot = i;
      // when found stop
      if (condition(i[0], i[1]))
      {
        found = true;
        break;
      }
    }
  }

  // if not found display flat line
  if (std::distance(firstSampleToPlot, currentlyDisplayedData.end()) <= 2)
  {
    std::fill(currentlyDisplayedData.begin(), currentlyDisplayedData.end(), 0.);
    firstSampleToPlot = currentlyDisplayedData.begin();
  }
}

void TriggeredOscilloscope::plot(juce::Graphics &g, juce::Rectangle<float> rect, float scaler, float offset)
{
  // get beginning and num samples
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();

  // get bounds
  auto w = rect.getWidth();
  auto h = rect.getHeight();
  auto right = rect.getRight();
  auto center = rect.getBottom() - offset;
  auto gain = h * scaler;
  g.setColour(WAVEFORMCOLOUR());

  // get distance of trigger point from beginning
  int sampleOffset = std::distance(data, firstSampleToPlot);

  // ratio sampleOffset / numSamples to map correctly waveform
  float widthRatio = sampleOffset / numSamples;

  // for each point map & draw line
  for (size_t i = 1; i < numSamples - sampleOffset; ++i)
  {
    auto xMapPrevious = juce::jmap(
        float(i - 1),
        float(sampleOffset),
        float(numSamples - 1),
        float(right - w),
        float(right * (1 - widthRatio)));
    auto xMap = juce::jmap(
        float(i),
        float(sampleOffset),
        float(numSamples - 1),
        float(right - w),
        float(right * (1 - widthRatio)));

    g.drawLine({xMapPrevious,
                center - gain * firstSampleToPlot[i - 1],
                xMap,
                center - gain * firstSampleToPlot[i]});
  }
}

void TriggeredOscilloscope::subclassSpecificCallback()
{
  // if refresh is needed refresh displayed data
  if (decayCounter.get()->refreshNeeded())
  {
    refreshDislayed();
  }
}
