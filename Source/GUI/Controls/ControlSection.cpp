/*
  ==============================================================================

    ControlSection.cpp
    Created: 8 Mar 2023 11:57:32am
    Author:  wadda

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ControlSection.h"

//==============================================================================

ControlSection::ControlSection()
{
    // make objects visible
    addAndMakeVisible(toggleOscilloscope);
    addAndMakeVisible(toggleText);
    addAndMakeVisible(basicControls);
    addAndMakeVisible(proControls);

    // set ControlSection expander button
    toggleText.setJustificationType(juce::Justification::centred);
    toggleText.setFont(juce::Font(18, 1));
    toggleText.setText("+", juce::dontSendNotification);
    toggleOscilloscope.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    toggleOscilloscope.setAlpha(0);
    toggleOscilloscope.setAlwaysOnTop(true);

    // set number of sections shown
    numHorizontalSections = 1;
}

ControlSection::~ControlSection()
{
}

void ControlSection::setAttachment(juce::String attachmentName, juce::AudioProcessorValueTreeState &processorTreeState)
{
    // resolve attachment name and map it to correct attachment
    switch (resolveAttachment(attachmentName))
    {
    case (ControlSection::Attachments::BufferLength):
    {
        basicControls.bufferLengthAttachment.reset(
            new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, basicControls.bufferLength));
        break;
    }
    case (ControlSection::Attachments::DrawGrid):
    {
        basicControls.gridAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, basicControls.drawGrid));
        break;
    }
    case (ControlSection::Attachments::IsProfessional):
    {
        isProfessionalAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, toggleOscilloscope));
        resetNumHorizontalSections();
        break;
    }
    case (ControlSection::Attachments::IsTriggered):
    {
        proControls.triggerButtonAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, proControls.triggerButton));

        // Set auto trigger visible if trigger selected
        proControls.autoTriggerButton.setVisible(proControls.triggerButton.getToggleState());
        break;
    }
    case (ControlSection::Attachments::TriggerLevel):
    {
        proControls.triggerLevelAttachment.reset(
            new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, proControls.triggerLevel));
        break;
    }
    case (ControlSection::Attachments::SlopeButtonTriggered):
    {
        proControls.slopeButtonAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, proControls.slopeButton));
        break;
    }
    case (ControlSection::Attachments::IsAutoTriggered):
    {
        proControls.autoTriggerButtonAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, proControls.autoTriggerButton));
        break;
    }
    case (ControlSection::Attachments::DecayTime):
    {
        proControls.refreshTimeAttachment.reset(
            new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, proControls.refreshTime));
        break;
    }
    case (ControlSection::Attachments::MuteOutput):
    {
        basicControls.muteOutputAttachment.reset(
            new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, basicControls.muteOutput));
        break;
    }
    default:
    {
        break;
    }
    }
}

void ControlSection::setMultipleAttachments(std::vector<juce::String> attachmentNames, juce::AudioProcessorValueTreeState &processorTreeState)
{
    // call setAttachment for each attachment in attachmentNames
    for (int i = 0; i < attachmentNames.size(); i++)
    {
        setAttachment(attachmentNames[i], processorTreeState);
    }
}

void ControlSection::resetNumHorizontalSections()
{
    // check state and set variable
    if (toggleOscilloscope.getToggleState())
    {
        numHorizontalSections = 2;
    }
    else
    {
        numHorizontalSections = 1;
    }
}

void ControlSection::resetButtonText()
{
    // check state and set text
    if (toggleOscilloscope.getToggleState())
    {
        toggleText.setText("-", juce::dontSendNotification);
    }
    else
    {
        toggleText.setText("+", juce::dontSendNotification);
    }
}

void ControlSection::paint(juce::Graphics &g)
{

    // fill background
    g.fillAll(CONTROLSECTIONCOLOR());

    // draw toggle ControlSection expander contour
    g.setColour(GUITOGGLERCOLOUR());
    g.drawRect(toggleOscilloscopeArea);
}

void ControlSection::resized()
{
    // set ControlSection expander area
    toggleOscilloscopeArea = juce::Rectangle<int>(
        getWidth() / 2 - 10,
        10,
        20,
        20);
    toggleOscilloscope.setBounds(toggleOscilloscopeArea);
    toggleText.setBounds(toggleOscilloscopeArea);

    // set basicControls position and size
    int basicControlHeight = getHeight() / numHorizontalSections;
    basicControls.setTopLeftPosition(0, 0);
    basicControls.setSize(getWidth(), basicControlHeight);

    // set proControls position and size
    proControls.setTopLeftPosition(0, basicControlHeight);
    proControls.setSize(getWidth(), getHeight() - basicControlHeight);
}

ControlSection::Attachments ControlSection::resolveAttachment(juce::String attachmentName)
{
    // map string to Attachment enum
    auto itr = attachmentsMap.find(attachmentName);
    if (itr != attachmentsMap.end())
    {
        return itr->second;
    }
    return ControlSection::Attachments::Default;
}
