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
    , sampleFinder(
      !aProcessor.getTreeState()->getParameterAsValue("slopeButtonTriggered").getValue(),
      aProcessor.getTreeState()->getParameterAsValue("autoTriggered").getValue()
    )
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

  // add sampleFinder listeners
  aProcessor.getTreeState()->addParameterListener("autoTriggered", &sampleFinder);
  aProcessor.getTreeState()->addParameterListener("slopeButtonTriggered", &sampleFinder.autoFinder);
  aProcessor.getTreeState()->addParameterListener("slopeButtonTriggered", &sampleFinder.manualFinder);

  // refresh displayed data
  refreshDislayed();
}

TriggeredOscilloscope::~TriggeredOscilloscope()
{
  // remove listeners
  audioProcessor.getTreeState()->removeParameterListener("decayTime", decayCounter.get());
  audioProcessor.getTreeState()->removeParameterListener("autoTriggered", &sampleFinder);
  audioProcessor.getTreeState()->removeParameterListener("slopeButtonTriggered", &sampleFinder.autoFinder);
  audioProcessor.getTreeState()->removeParameterListener("slopeButtonTriggered", &sampleFinder.manualFinder);
}

void TriggeredOscilloscope::refreshDislayed()
{
  // resize buffer and copy from sampleData
  auto size = sampleData.size();
  currentlyDisplayedData.resize(sampleData.size());
  std::copy(sampleData.begin(), sampleData.end(), currentlyDisplayedData.begin());

  // get trigger level
  triggerLevel = audioProcessor.getTreeState()->getParameterAsValue("triggerLevel").getValue();

  // reset firstSampleToPlot
  const int offset = sampleFinder.findFirstSample(triggerLevel, currentlyDisplayedData);
  firstSampleToPlot = currentlyDisplayedData.begin() + offset;

  // if not found display flat line
  if (offset == -1)
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
