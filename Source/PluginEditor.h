/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OscilloscopeComponent.h"

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

    OscilloscopeAudioProcessor& audioProcessor;
    OscilloscopeComponent<float> oscilloscopeComponent;
    juce::ToggleButton drawGrid;
    juce::AudioProcessorValueTreeState::ButtonAttachment* gridAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* bufferLengthAttachment;
    juce::ComboBox strategySelector;
    juce::Slider bufferLength;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscilloscopeAudioProcessorEditor)
};
