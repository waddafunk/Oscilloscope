/*
  ==============================================================================

    SampleFinder.h
    Created: 2 Apr 2023 5:30:47pm
    Author:  covariant

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
 * Methods to find the first sample to plot when in triggered mode.
 *
 */
namespace FindMethods
{
  /**
   * Find first sample to plot with crescent slope.
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot.
   */
  int crescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);

  /**
   * Find first sample to plot with crescent slope in automatic mode.
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot.
   */
  int autoCrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);

  /**
   * Find first sample to plot with decrescent slope
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot.
   */
  int decrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);

  /**
   * Find first sample to plot with decrescent slope in automatic mode
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot.
   */
  int autoDecrescentFirst(float triggerLevel, std::vector<float> currentlyDisplayedData);
};

/**
 * Virtual base class from where to inherit to implement all cases of how
 * to find the first sample to plot.
 *
 */
class BaseFinder : public juce::AudioProcessorValueTreeState::Listener
{
public:
  /**
   *Find first sample to plot in the current modality.
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot or -1 if no
   * feasible sample is found.
   */
  int findFirstSample(float triggerLevel, std::vector<float> currentlyDisplayedData);

protected:
  /**
   * Lambda triggering the element of FindMethods corresponding to the
   * current find modality
   *
   */
  std::function<int(float, std::vector<float>)> findSample;
};

/**
 * Class finding the first sample to plot in automatic mode.
 *
 */
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

/**
 * Class finding the first sample to plot in manual mode.
 *
 */
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

/**
 * Class finding first sample to plot handling the logic for all modalities.
 * Add this and its autoFinder and manualFinder params as listeners of
 * "autoTriggered" and "slopeButtonTriggered" respectively and just
 * call findFirstSample.
 *
 */
class SampleFinder : public juce::AudioProcessorValueTreeState::Listener
{
public:
  /**
   * Construct a new Sample Finder object.
   *
   * \param isCrescent Whether the slope is crescent.
   * \param isAuto Whether the mode is automatic.
   */
  SampleFinder(bool isCrescent, bool isAuto);

  /**
   * Destroy the Sample Finder object
   *
   */
  ~SampleFinder();

  /**
   * Find first sample to plot in the current modality.
   *
   * \param triggerLevel Trigger level.
   * \param currentlyDisplayedData Currently displayed buffer.
   * \return int Distance in samples of first sample to plot.
   */
  int findFirstSample(float triggerLevel, std::vector<float> currentlyDisplayedData);

  /**
   * Sets finder according to whether the search is automatic or manual.
   *
   * \param parameterID Parameter to listen (always "autoTriggered")
   * \param newValue true if automatic, false otherwise
   */
  void parameterChanged(const juce::String &parameterID, float newValue) override;

  /**
   * Finds first sample when in automatic mode.
   *
   */
  AutoSampleFinder autoFinder;

  /**
   * Finds first sample when in manual mode.
   *
   */
  ManualSampleFinder manualFinder;

  /**
   * Points to the currently active finder.
   *
   */
  BaseFinder *currentFinder;
};
