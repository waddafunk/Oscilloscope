/*
  ==============================================================================

    ControlSection.h
    Created: 8 Mar 2023 11:57:32am
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ColorPalette.h"
#include "InitVariables.h"

//==============================================================================
/**
 * Class handling the GUI controls.
 */
class ControlSection  : public juce::Component
{
public:
    ControlSection();
    ~ControlSection() override;

    /**
     * Get if grid box is ticked.
     * 
     * \return boolean telling whether grid box is ticked.
     */
    bool getDrawGrid();

    /**
     * Set lambda function to be triggered by ticking grid box.
     * 
     * \param drawGridLambda Lambda function to trigger.
     */
    void setDrawGridStateChange(std::function<void()> drawGridLambda);
    /**
     * Set attachment between control and treestate.
     * 
     * \param attachmentName Attachment name, @see attachmentMap
     * \param processorTreeState Audio processor's Treestate.
     */
    void setAttachment(juce::String attachmentName, juce::AudioProcessorValueTreeState& processorTreeState);
    /**
     * Set multiple attachments.
     * 
     * \param attachmentNames Vector of attachment names.
     * \param processorTreeState Audio processor's Treestate.
     */
    void setMultipleAttachments(std::vector<juce::String> attachmentNames, juce::AudioProcessorValueTreeState& processorTreeState);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    /**
     * List of attachments.
     */
    enum Attachments
    {
        BufferLength,
        DrawGrid,
        IsProfessional,
        Default
    };
    /**
     * Mapping from attachments' names to Attachments enum class.
     */
    const std::map<juce::String, ControlSection::Attachments> attachmentsMap{
        { "drawGrid", ControlSection::Attachments::DrawGrid },
        { "bufferLength", ControlSection::Attachments::BufferLength },
        { "isProfessional", ControlSection::Attachments::IsProfessional },
    };

    /**
     * Area where the oscilloscope toggle button is placed.
     * 
     */
    juce::Rectangle<int> toggleOscilloscopeArea; 

    /**
     * Returns the proper attachment if a correct name is passed, Attachments::Default otherwise..
     * 
     * \param attachmentName Attachment name.
     * \return proper Attachments value.
     */
    Attachments resolveAttachment(juce::String attachmentName);

    juce::ToggleButton drawGrid;
    juce::Slider bufferLength;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> bufferLengthAttachment;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> gridAttachment;
    juce::ToggleButton toggleOscilloscope;
    juce::Label toggleText;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlSection)
};
