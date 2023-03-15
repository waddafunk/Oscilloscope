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
    toggleText.setJustificationType(juce::Justification::centred);
    toggleText.setText("Pro", juce::dontSendNotification);
    toggleOscilloscope.setMouseCursor(juce::MouseCursor::PointingHandCursor);
    toggleOscilloscope.setAlpha(0);
    toggleOscilloscope.setAlwaysOnTop(true);
    //toggleOscilloscope.setColour(juce::Colours::white.withMultipliedSaturation(0));

    addAndMakeVisible(drawGrid);
    addAndMakeVisible(bufferLength);
    drawGrid.setButtonText("Grid");

    bufferLength.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    bufferLength.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
}

ControlSection::~ControlSection()
{
}

bool ControlSection::getDrawGrid()
{
    return drawGrid.getToggleState();
}

void ControlSection::setDrawGridStateChange(std::function<void()> drawGridLambda)
{
    drawGrid.onStateChange = drawGridLambda;
}

void ControlSection::setAttachment(juce::String attachmentName, juce::AudioProcessorValueTreeState& processorTreeState)
{
    switch (resolveAttachment(attachmentName))
    {
        case (ControlSection::Attachments::BufferLength):
            {
                bufferLengthAttachment.reset(
                    new juce::AudioProcessorValueTreeState::SliderAttachment(processorTreeState, attachmentName, bufferLength)
                );
                break;
            }
        case (ControlSection::Attachments::DrawGrid):
            {
                gridAttachment.reset(
                    new juce::AudioProcessorValueTreeState::ButtonAttachment(processorTreeState, attachmentName, drawGrid)
                );
                break;
            }
        case (ControlSection::Attachments::IsProfessional):
            {
                gridAttachment.reset(
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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (CONTROLSECTIONCOLOR()); 

    g.setColour(juce::Colours::dimgrey);
    auto contour = juce::Line<float>(0, 0, getWidth(), 0);
    g.drawLine(contour, 8.0f);

    g.fillRect(toggleOscilloscopeArea);

}

void ControlSection::resized()
{
    toggleOscilloscopeArea = juce::Rectangle<int>(getWidth() / 2 - getWidth() / 40, 0, getWidth() / 20, getHeight() / 3);
    toggleOscilloscope.setBounds(toggleOscilloscopeArea);
    toggleText.setBounds(toggleOscilloscopeArea);
    drawGrid.setSize(getWidth() / 5., getHeight() * 3. / 4.);
    drawGrid.setTopLeftPosition(10, getHeight() / 8.);
    bufferLength.setSize(getWidth() * 2 / 5, getHeight() * 3. / 4.);
    bufferLength.setTopLeftPosition(getWidth() * 11. / 20., getHeight() / 8.);
}

ControlSection::Attachments ControlSection::resolveAttachment(juce::String attachmentName)
{
    auto itr = attachmentsMap.find(attachmentName);
    if (itr != attachmentsMap.end()) {
        return itr->second;
    }
    return ControlSection::Attachments::Default;
}
