/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscilloscopeAudioProcessorEditor::OscilloscopeAudioProcessorEditor (OscilloscopeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    oscilloscopeComponent.reset(new UntriggeredOscilloscope(audioProcessor, audioProcessor.getSampleRate()));

    addAndMakeVisible(oscilloscopeComponent.get());
    addAndMakeVisible(controlSection);
    
    setSize(audioProcessor.getEditorWidth(), audioProcessor.getEditorHeight());

    auto area = getLocalBounds();
    
    oscilloscopeComponent->setTopLeftPosition(0, 0);
    oscilloscopeComponent->setSize(area.getWidth(), area.getHeight() - 30);
    controlSection.setDrawGridStateChange([this]() {
        auto draw = controlSection.getDrawGrid();
        oscilloscopeComponent->setGridCheck(draw);
    } );

    controlSection.setTopLeftPosition(0, area.getHeight() - 30);
    controlSection.setSize(area.getWidth(), 30);
    // Attach the ToggleButton to the AudioParameterBool
    std::vector<juce::String> attachmentNames;
    attachmentNames.push_back("drawGrid");
    attachmentNames.push_back("bufferLength");
    controlSection.setMultipleAttachments(attachmentNames, *audioProcessor.getTreeState());
    
    setResizable(true, true);
    setResizeLimits(256, 256, 1920, 1080);
    resized(); // need to be called in constructor since rescaling logic makes oscilloscope invisible on creation
    
}

OscilloscopeAudioProcessorEditor::~OscilloscopeAudioProcessorEditor()
{
}

//==============================================================================
void OscilloscopeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(95, 2, 31).darker(0.8));

}

void OscilloscopeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getLocalBounds();
    int height = area.getHeight();
    int width = area.getWidth();

    // rescale oscilloscope
    float x_sc = (float)width / oscilloscopeComponent->getWidth();
    float y_sc = (float)(height - 30) / oscilloscopeComponent->getHeight();
    oscilloscopeComponent->setTransform(juce::AffineTransform::scale(x_sc, y_sc));

    // translate & rescale control section
    float yDiff = (float)(height - 30) - oscilloscopeComponent->getHeight();
    y_sc = 30. / controlSection.getHeight();
    controlSection.setTransform(juce::AffineTransform::translation(0, yDiff));
    //controlSection.setTransform(juce::AffineTransform::scale(x_sc, y_sc));

    // store new size
    audioProcessor.storeEditorSize(getWidth(), getHeight());
    
}


