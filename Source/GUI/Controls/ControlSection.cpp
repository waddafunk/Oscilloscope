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
    addAndMakeVisible(toggleOscilloscope);
    addAndMakeVisible(toggleText);
    addAndMakeVisible(basicControls);
    addAndMakeVisible(proControls);

    //basicControls.setAlwaysOnTop(true);

    toggleText.setJustificationType(juce::Justification::centred);
    toggleText.setFont(juce::Font(18, 1));
    toggleText.setText("+", juce::dontSendNotification);
    toggleOscilloscope.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    toggleOscilloscope.setAlpha(0);
    toggleOscilloscope.setAlwaysOnTop(true);

    numHorizontalSections = 1;
}

ControlSection::~ControlSection()
{
}

void ControlSection::setAttachment(juce::String attachmentName, juce::AudioProcessorValueTreeState& processorTreeState)
{
    switch (resolveAttachment(attachmentName))
    {
        case (ControlSection::Attachments::BufferLength):
            {
                basicControls.bufferLengthAttachment.reset(
                    new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, basicControls.bufferLength)
                );
                break;
            }
        case (ControlSection::Attachments::DrawGrid):
            {
                basicControls.gridAttachment.reset(
                    new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, basicControls.drawGrid)
                );
                break;
            }
        case (ControlSection::Attachments::IsProfessional):
            {
                isProfessionalAttachment.reset(
                    new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, toggleOscilloscope)
                );
                resetNumHorizontalSections();
                break;
            }
        case (ControlSection::Attachments::IsTriggered):
            {
                proControls.triggerButtonAttachment.reset(
                    new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, proControls.triggerButton)
                );
                break;
            }
        case (ControlSection::Attachments::TriggerLevel):
            {
                proControls.triggerLevelAttachment.reset(
                    new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, proControls.triggerLevel)
                );
                break;
            }
        case (ControlSection::Attachments::SlopeButtonTriggered):
            {
                proControls.slopeButtonAttachment.reset(
                    new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, proControls.slopeButton)
                );
                break;
            }
        default:
            {
                break;
            }
    }
}

void ControlSection::setMultipleAttachments(std::vector<juce::String> attachmentNames, juce::AudioProcessorValueTreeState& processorTreeState)
{
    for (int i = 0; i < attachmentNames.size(); i++)
    {
        setAttachment(attachmentNames[i], processorTreeState);
    }
}

void ControlSection::resetNumHorizontalSections()
{
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

    g.fillAll (CONTROLSECTIONCOLOR()); 

    g.setColour(GUITOGGLERCOLOUR());

    g.drawRect(toggleOscilloscopeArea);

}

void ControlSection::resized()
{
    toggleOscilloscopeArea = juce::Rectangle<int>(
        getWidth() / 2 - 10, 
        10, 
        20, 
        20
    );
    toggleOscilloscope.setBounds(toggleOscilloscopeArea);
    toggleText.setBounds(toggleOscilloscopeArea);

    int basicControlHeight = getHeight() / numHorizontalSections;
    basicControls.setTopLeftPosition(0, 0);
    basicControls.setSize(getWidth(), basicControlHeight);

    proControls.setTopLeftPosition(0, basicControlHeight);
    proControls.setSize(getWidth(), getHeight() - basicControlHeight);
}

ControlSection::Attachments ControlSection::resolveAttachment(juce::String attachmentName)
{
    auto itr = attachmentsMap.find(attachmentName);
    if (itr != attachmentsMap.end()) {
        return itr->second;
    }
    return ControlSection::Attachments::Default;
}
