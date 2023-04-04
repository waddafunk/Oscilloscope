/*
  ==============================================================================

    SampleFinder.h
    Created: 2 Apr 2023 5:30:47pm
    Author:  covariant

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace FindMethods
{
  int crescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);
  int autoCrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);
  int decrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);
  int autoDecrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);
};

class BaseFinder : public juce::AudioProcessorValueTreeState::Listener
{
public:
  int findFirstSample(float triggerLevel, std::vector<float> currentlyDisplayedData);

protected:
  std::function<int (float, std::vector<float>)> findSample;
};

class AutoSampleFinder : public BaseFinder
{

public:
  /**
   * Sets findSample accordingly to whether to search for crescent or
   * decrescent slope
   *
   * \param parameterID Parameter to listen (always "slopeButtonTriggered")
   * \param newValue true if the plot has to be decrescent, false otherwise
   */
  void parameterChanged(const juce::String &parameterID, float newValue) override;
};

class ManualSampleFinder : public BaseFinder
{
public:
  /**
   * Sets findSample accordingly to whether to search for crescent or
   * decrescent slope
   *
   * \param parameterID Parameter to listen (always "slopeButtonTriggered")
   * \param newValue true if the plot has to be decrescent, false otherwise
   */
  void parameterChanged(const juce::String &parameterID, float newValue) override;
};

class SampleFinder : public juce::AudioProcessorValueTreeState::Listener
{
public:
  SampleFinder(bool isCrescent, bool isAuto);
  ~SampleFinder();

  int findFirstSample(float triggerLevel, std::vector<float> currentlyDisplayedData);

  /**
   * Sets finder according to whether the search is automatic or manual.
   *
   * \param parameterID Parameter to listen (always "autoTriggered")
   * \param newValue true if automatic, false otherwise
   */
  void parameterChanged(const juce::String &parameterID, float newValue) override;

  AutoSampleFinder autoFinder;
  ManualSampleFinder manualFinder;
  BaseFinder *currentFinder;
};
