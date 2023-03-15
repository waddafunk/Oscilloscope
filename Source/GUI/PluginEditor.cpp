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

    // add and make visible components
    addAndMakeVisible(oscilloscopeComponent.get());
    addAndMakeVisible(controlSection);
    
    // set editor size
    setSize(audioProcessor.getEditorWidth(), audioProcessor.getEditorHeight());

    controlSection.setDrawGridStateChange([this]() {
        auto draw = controlSection.getDrawGrid();
        oscilloscopeComponent->setGridCheck(draw);
    } );

    // set control section attachments
    std::vector<juce::String> attachmentNames;
    attachmentNames.push_back("drawGrid");
    attachmentNames.push_back("bufferLength");
    attachmentNames.push_back("isProfessional");
    controlSection.setMultipleAttachments(attachmentNames, *audioProcessor.getTreeState());
    
    // set resize options
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

}

void OscilloscopeAudioProcessorEditor::resized()
{
    // get bounds
    auto area = getLocalBounds();
    int height = area.getHeight();
    int width = area.getWidth();
    int margin = int(float(area.getHeight()) * 12. / 13.);

    // resize components
    oscilloscopeComponent->setTopLeftPosition(0, 0);
    oscilloscopeComponent->setSize(area.getWidth(), margin);
    controlSection.setTopLeftPosition(0, margin);
    controlSection.setSize(width, height - margin);

    // store new size
    audioProcessor.storeEditorSize(getWidth(), getHeight());
    
}


