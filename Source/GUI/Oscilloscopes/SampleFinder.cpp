/*
  ==============================================================================

    SampleFinder.cpp
    Created: 2 Apr 2023 5:30:47pm
    Author:  covariant

  ==============================================================================
*/

#include "SampleFinder.h"

void AutoSampleFinder::parameterChanged(const juce::String &parameterID, float newValue)
{
  // if decrescent
  if (newValue)
  {
    this->findSample = [](float triggerLevel, std::vector<float> currentlyDisplayedData)
    { return FindMethods::autoDecrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
  // if crescent
  else
  {
    this->findSample = [](float triggerLevel, std::vector<float> currentlyDisplayedData)
    { return FindMethods::autoCrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
}

void ManualSampleFinder::parameterChanged(const juce::String &parameterID, float newValue)
{
  // if decrescent
  if (newValue)
  {
    this->findSample = [](float triggerLevel, std::vector<float> currentlyDisplayedData)
    { return FindMethods::decrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
  // if crescent
  else
  {
    this->findSample = [](float triggerLevel, std::vector<float> currentlyDisplayedData)
    { return FindMethods::crescentFirst(triggerLevel, currentlyDisplayedData); };
  }
}

SampleFinder::SampleFinder(bool isCrescent, bool isAuto)
{
  // set current state for all listeners
  parameterChanged("", isAuto);
  autoFinder.parameterChanged("", isCrescent);
  manualFinder.parameterChanged("", isCrescent);
}

SampleFinder::~SampleFinder() {}

int SampleFinder::findFirstSample(float triggerLevel,
                                  std::vector<float> currentlyDisplayedData)
{
  // just return the value found from the currently active finder
  return this->currentFinder->findFirstSample(triggerLevel, currentlyDisplayedData);
}

void SampleFinder::parameterChanged(const juce::String &parameterID, float newValue)
{
  // if automatic
  if (newValue)
  {
    currentFinder = &autoFinder;
  }
  // if manual
  else
  {
    currentFinder = &manualFinder;
  }
}

int FindMethods::crescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData)
{
  // declare variables
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();
  std::vector<float>::iterator firstToPlot;

  // condition to stop searching (lamda func)
  auto condition = [](float x, float subseq, float trig)
  {
    bool firstCondition = std::abs(x - trig) < 0.05;
    bool secondCondition = subseq > x;
    return firstCondition && secondCondition;
  };

  // search in all currentlyDisplayedData
  for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
  {
    firstToPlot = i;
    // when found return
    if (condition(i[0], i[1], triggerLevel))
    {
      int distance = std::distance(currentlyDisplayedData.begin(), firstToPlot);
      return distance;
    }
  }
  // if not found return -1
  int notFound = -1;
  return notFound;
}

int FindMethods::autoCrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData)
{
  // declare variables
  float max = *std::max_element(currentlyDisplayedData.begin(), currentlyDisplayedData.end());
  float triggerPoint = max * triggerLevel;
  std::vector<float>::iterator firstToPlot;

  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();

  // condition to stop searching (lamda func)
  auto condition = [](float x, float subseq, float trig)
  {
    bool firstCondition = std::abs(x - trig) < 0.05;
    bool secondCondition = subseq > x;
    return firstCondition && secondCondition;
  };

  // search in all currentlyDisplayedData
  for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
  {
    firstToPlot = i;
    // when found stop
    if (condition(i[0], i[1], triggerPoint))
    {
      int distance = std::distance(currentlyDisplayedData.begin(), firstToPlot);
      return distance;
    }
  }

  // if not found return -1
  int notFound = -1;
  return notFound;
}

int FindMethods::decrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData)
{
  // declare variables
  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();
  std::vector<float>::iterator firstToPlot;

  // condition to stop searching (lamda func)
  auto condition = [](float x, float subseq, float trig)
  {
    bool firstCondition = std::abs(x - trig) < 0.05;
    bool secondCondition = subseq < x;
    return firstCondition && secondCondition;
  };

  // search in all currentlyDisplayedData
  for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
  {
    firstToPlot = i;
    // when found stop
    if (condition(i[0], i[1], triggerLevel))
    {
      int distance = std::distance(currentlyDisplayedData.begin(), firstToPlot);
      return distance;
    }
  }

  // if not found return -1
  int notFound = -1;
  return notFound;
}

int FindMethods::autoDecrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData)
{
  // declare variables
  float max = *std::max_element(currentlyDisplayedData.begin(), currentlyDisplayedData.end());
  float triggerPoint = max * triggerLevel;
  std::vector<float>::iterator firstToPlot;

  auto data = currentlyDisplayedData.begin();
  auto numSamples = currentlyDisplayedData.size();

  // condition to stop searching (lamda func)
  auto condition = [](float x, float subseq, float trig)
  {
    bool firstCondition = std::abs(x - trig) < 0.05;
    bool secondCondition = subseq < x;
    return firstCondition && secondCondition;
  };

  // search in all currentlyDisplayedData
  for (auto i = currentlyDisplayedData.begin(); i < currentlyDisplayedData.end() - 1; ++i)
  {
    firstToPlot = i;
    // when found stop
    if (condition(i[0], i[1], triggerPoint))
    {
      int distance = std::distance(currentlyDisplayedData.begin(), firstToPlot);
      return distance;
    }
  }

  // if not found return -1
  int notFound = -1;
  return notFound;
}

int BaseFinder::findFirstSample(float triggerLevel, std::vector<float> currentlyDisplayedData)
{
  // Return the value found from the currently active finder.
  return this->findSample(triggerLevel, currentlyDisplayedData);
}
