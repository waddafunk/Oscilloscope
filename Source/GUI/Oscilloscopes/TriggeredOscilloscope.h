/*
  ==============================================================================

    TriggeredOscilloscope.h
    Created: 28 Feb 2023 4:24:56pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include "OscilloscopeComponent.h"
#include "DecayCounter.h"
#include "InitVariables.h"
#include "ColorPalette.h"
#include "SampleFinder.h"

class TriggeredOscilloscope : public OscilloscopeComponent
{

public:
  /**
   * Construct a new Triggered Oscilloscope object
   *
   * \param aProcessor PluginProcessor audio processor.
   * \param sampleRate Sample rate of the host.
   */
  TriggeredOscilloscope(OscilloscopeAudioProcessor &aProcessor, int sampleRate);

  /**
   * Destroy the Triggered Oscilloscope object
   *
   */
  ~TriggeredOscilloscope();

  /**
   * Refreshes displayed data and finds first element to plot.
   *
   */
  void refreshDislayed();

private:
  /**
   * Current trigger level.
   *
   */
  float triggerLevel;

  /**
   * Class keeping track of decay time.
   *
   */
  std::unique_ptr<DecayCounter> decayCounter;

  /**
   * Data currently displayed.
   *
   */
  std::vector<float> currentlyDisplayedData;

  /**
   * First sample to plot.
   *
   */
  std::vector<float>::iterator firstSampleToPlot;

  /**
   * Class handling the logic to find the first sample to plot.
   *
   */
  SampleFinder sampleFinder;

  void plot(
      juce::Graphics &g,
      juce::Rectangle<float> rect,
      float scaler = float(1),
      float offset = float(0)) override;

  void subclassSpecificCallback() override;
};
