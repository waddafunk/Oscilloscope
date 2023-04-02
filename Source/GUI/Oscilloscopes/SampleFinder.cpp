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
  if (newValue)
  {
    this->findSample = [](float triggerLevel, std::vector<float> &currentlyDisplayedData)
    { return FindMethods::autoDecrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
  else
  {
    this->findSample = [](float triggerLevel, std::vector<float> &currentlyDisplayedData)
    { return FindMethods::autoCrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
}

void ManualSampleFinder::parameterChanged(const juce::String &parameterID, float newValue)
{
  if (newValue)
  {
    this->findSample = [](float triggerLevel, std::vector<float> &currentlyDisplayedData)
    { return FindMethods::decrescentFirst(triggerLevel, currentlyDisplayedData); };
  }
  else
  {
    this->findSample = [](float triggerLevel, std::vector<float> &currentlyDisplayedData)
    { return FindMethods::crescentFirst(triggerLevel, currentlyDisplayedData); };
  }
}

std::vector<float>::iterator SampleFinder::findFirstSample(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{
  return this->currentFinder->findFirstSample(triggerLevel, currentlyDisplayedData);
}

void SampleFinder::parameterChanged(const juce::String &parameterID, float newValue)
{
  if (newValue)
  {
    currentFinder = &autoFinder;
  }
  else
  {
    currentFinder = &manualFinder;
  }
}

std::vector<float>::iterator FindMethods::crescentFirst(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{

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
    // when found stop
    if (condition(i[0], i[1], triggerLevel))
    {
      break;
    }
  }

  return firstToPlot;
}

std::vector<float>::iterator FindMethods::autoCrescentFirst(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{
  float max = *std::max_element(currentlyDisplayedData.begin(), currentlyDisplayedData.end());
  float triggerPoint = max * triggerLevel;

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
    // when found stop
    if (condition(i[0], i[1], triggerPoint))
    {
      break;
    }
  }

  return firstToPlot;
}

std::vector<float>::iterator FindMethods::decrescentFirst(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{
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
      break;
    }
  }

  return firstToPlot;
}

std::vector<float>::iterator FindMethods::autoDecrescentFirst(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{
  float max = *std::max_element(currentlyDisplayedData.begin(), currentlyDisplayedData.end());
  float triggerPoint = max * triggerLevel;

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
    if (condition(i[0], i[1], triggerPoint))
    {
      break;
    }
  }

  return firstToPlot;
}

std::vector<float>::iterator BaseFinder::findFirstSample(float triggerLevel, std::vector<float> &currentlyDisplayedData)
{
  return this->findSample(triggerLevel, currentlyDisplayedData);
}
