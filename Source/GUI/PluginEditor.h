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
#include "ControlSection.h"
#include "GuiTransformer.h"

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
    ControlSection controlSection;
    std::unique_ptr<OscilloscopeComponent> oscilloscopeComponent;
    std::unique_ptr<GuiTransformer> guiTransformer;

    float margin_multiplier;
    void expandCallback();
    void expansionEndedCallback();
    void contractCallback();
    void contractionEndedCallback();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscilloscopeAudioProcessorEditor)
};
