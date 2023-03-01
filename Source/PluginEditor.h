/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscilloscopeComponent.h"
#include"UntriggeredOscilloscope.h"

//==============================================================================
/**
*/
class OscilloscopeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OscilloscopeAudioProcessorEditor (OscilloscopeAudioProcessor&);
    ~OscilloscopeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OscilloscopeAudioProcessor& audioProcessor;
    std::unique_ptr<OscilloscopeComponent> oscilloscopeComponent;
    juce::ToggleButton drawGrid;
    juce::Slider bufferLength;
    juce::AudioProcessorValueTreeState::SliderAttachment* bufferLengthAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment* gridAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscilloscopeAudioProcessorEditor)
};
