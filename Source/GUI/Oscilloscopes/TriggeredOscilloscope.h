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

class TriggeredOscilloscope : public OscilloscopeComponent
{

public:

  /**
   * Construct a new Triggered Oscilloscope object
   * 
   * \param aProcessor PluginProcessor audio processor.
   * \param sampleRate Sample rate of the host.
   */
  TriggeredOscilloscope(OscilloscopeAudioProcessor& aProcessor, int sampleRate);
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
   * Finds closest element to @ref value of @ref vec. 
   * 
   * \param vec Vector where to search closest value.
   * \param value Reference value.
   * \return std::vector<float>::iterator Iterator pointing to closest value
   */
  std::vector<float>::iterator closest(std::vector<float>::iterator begin, std::vector<float>::iterator end, float value);
  
  void plot(
      juce::Graphics& g,
      juce::Rectangle<float> rect,
      float scaler = float(1),
      float offset = float(0)) override;

  void subclassSpecificCallback() override;

};

