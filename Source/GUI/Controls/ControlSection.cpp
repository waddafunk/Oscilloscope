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

    //basicControls.setAlwaysOnTop(true);

    toggleText.setJustificationType(juce::Justification::centred);
    toggleText.setText("Pro", juce::dontSendNotification);
    toggleOscilloscope.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    toggleOscilloscope.setAlpha(0);
    toggleOscilloscope.setAlwaysOnTop(true);
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

void ControlSection::paint (juce::Graphics& g)
{

    g.fillAll (CONTROLSECTIONCOLOR()); 

    g.setColour(juce::Colours::dimgrey);
    auto contour = juce::Line<float>(0, 0, getWidth(), 0);
    g.drawLine(contour, 8.0f);

    g.fillRect(toggleOscilloscopeArea);

}

void ControlSection::resized()
{
    toggleOscilloscopeArea = juce::Rectangle<int>(getWidth() / 2 - getWidth() / 40, 0, getWidth() / 20, std::min(getHeight() / 3, 20));
    toggleOscilloscope.setBounds(toggleOscilloscopeArea);
    toggleText.setBounds(toggleOscilloscopeArea);

    basicControls.setTopLeftPosition(0, 0);
    basicControls.setSize(getWidth(), getHeight());
}

ControlSection::Attachments ControlSection::resolveAttachment(juce::String attachmentName)
{
    auto itr = attachmentsMap.find(attachmentName);
    if (itr != attachmentsMap.end()) {
        return itr->second;
    }
    return ControlSection::Attachments::Default;
}
