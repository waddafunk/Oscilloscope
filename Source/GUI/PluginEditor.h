/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscilloscopeComponent.h"
#include "UntriggeredOscilloscope.h"
#include "TriggeredOscilloscope.h"
#include "ControlSection.h"
#include "GuiTransformer.h"
#include "TriggerListener.h"

//==============================================================================
/**
 */
class OscilloscopeAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
  OscilloscopeAudioProcessorEditor(OscilloscopeAudioProcessor &);
  ~OscilloscopeAudioProcessorEditor() override;

  //==============================================================================
  void paint(juce::Graphics &) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  OscilloscopeAudioProcessor &audioProcessor;

  /**
   * Control Section component.
   *
   */
  ControlSection controlSection;

  /**
   * Gui transformer. Handles expansion & contraction of
   * \ref controlSection.
   *
   */
  std::unique_ptr<GuiTransformer> guiTransformer;

  /**
   * TriggerListener. Handles whether to plot in trigger mode or not.
   *
   */
  std::unique_ptr<TriggerListener> triggerListener;

  /**
   * Currently plotted oscilloscope. Can be TriggeredOscilloscope
   * or UntriggeredOscilloscope.
   *
   */
  std::unique_ptr<OscilloscopeComponent> oscilloscopeComponent;

  /**
   * Display component heigth over PluginEditor heigth
   *
   */
  float margin_multiplier;

  // GUI animation callbacks
  void expandCallback();
  void expansionEndedCallback();
  void contractCallback();
  void contractionEndedCallback();
  void expansionStartedCallback();
  void contractionStartedCallback();

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeAudioProcessorEditor)
};
