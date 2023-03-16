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

  // set correct oscilloscopeComponent height
  if (audioProcessor.getTreeState()->getParameterAsValue("isProfessional").getValue())
  {
    margin_multiplier = GUI_EXPANDED_MARGIN_MULTIPLIER();
  }
  else
  {
    margin_multiplier = GUI_CONTRACTED_MARGIN_MULTIPLIER();
  }

  // reset oscilloscopeComponent and guiTransformer pointers
  oscilloscopeComponent.reset(new UntriggeredOscilloscope(audioProcessor, audioProcessor.getSampleRate()));
  guiTransformer.reset(new GuiTransformer(
    audioProcessor,
    GUI_EXPAND_ANIMATION_DURATION(),
    [this] () { this->expandCallback(); },
    [this] () { this->contractCallback(); },
    [this] () { this->expansionStartedCallback(); },
    [this] () { this->contractionStartedCallback(); },
    [this] () { this->expansionEndedCallback(); },
    [this] () { this->contractionEndedCallback(); }
  ));

  // add and make visible components
  addAndMakeVisible(oscilloscopeComponent.get());
  addAndMakeVisible(controlSection);
  
  // set editor size
  setSize(audioProcessor.getEditorWidth(), audioProcessor.getEditorHeight());
  
  // set control section attachments
  std::vector<juce::String> attachmentNames;
  attachmentNames.push_back("drawGrid");
  attachmentNames.push_back("bufferLength");
  attachmentNames.push_back("isProfessional");
  controlSection.setMultipleAttachments(attachmentNames, *audioProcessor.getTreeState());
  
  // set resize options
  setResizable(true, true);
  setResizeLimits(256, 256, 1920, 1080);
  
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
    int margin = int(float(area.getHeight()) * margin_multiplier);

    // resize components
    oscilloscopeComponent->setTopLeftPosition(0, 0);
    oscilloscopeComponent->setSize(area.getWidth(), margin);
    controlSection.setTopLeftPosition(0, margin);
    controlSection.setSize(width, height - margin);

    // store new size
    audioProcessor.storeEditorSize(getWidth(), getHeight());
    
}

void OscilloscopeAudioProcessorEditor::expandCallback()
{
  
  if (this->margin_multiplier > GUI_EXPANDED_MARGIN_MULTIPLIER())
  {
    // increase by a fraction of the difference in pixels such that it takes the correct amount of frames
    float difference = GUI_CONTRACTED_MARGIN_MULTIPLIER() - GUI_EXPANDED_MARGIN_MULTIPLIER();
    float totalFrames = float(EDITOR_INITIAL_RATE()) * GUI_EXPAND_ANIMATION_DURATION();
    float increment = difference / totalFrames;
    this->margin_multiplier = this->margin_multiplier - increment;
  } 
  else
  {
    // set it to correct value
    this->margin_multiplier = GUI_EXPANDED_MARGIN_MULTIPLIER();
  }
  
  // reset component bounds
  this->resized();

}

void OscilloscopeAudioProcessorEditor::expansionEndedCallback()
{
  // reset component bounds
  this->resized(); 
}

void OscilloscopeAudioProcessorEditor::contractCallback()
{
  
  if (this->margin_multiplier < GUI_CONTRACTED_MARGIN_MULTIPLIER())
  {
    // increase by a fraction of the difference in pixels such that it takes the correct amount of frames
    float difference = GUI_CONTRACTED_MARGIN_MULTIPLIER() - GUI_EXPANDED_MARGIN_MULTIPLIER();;
    float totalFrames = float(EDITOR_INITIAL_RATE()) * GUI_EXPAND_ANIMATION_DURATION();
    float increment = difference / totalFrames;
    this->margin_multiplier = this->margin_multiplier + increment;
  } 
  else
  {
    // set it to correct value
    this->margin_multiplier = GUI_CONTRACTED_MARGIN_MULTIPLIER();
  }
  
  // reset component bounds
  this->resized();

}

void OscilloscopeAudioProcessorEditor::contractionEndedCallback()
{
  
  // reset component bounds
  this->resized();

}

void OscilloscopeAudioProcessorEditor::expansionStartedCallback()
{
  // reset component bounds
  this->resized();
}

void OscilloscopeAudioProcessorEditor::contractionStartedCallback()
{
  // reset component bounds
  this->resized();
}
