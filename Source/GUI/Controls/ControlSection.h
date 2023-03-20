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
#include "BasicControls.h"
#include "ProControls.h"

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

    /**
     * Resets @param numHorizontalSections in order to reflect the number
     * of horizontal control sections to display (i.e. 1 if not professional view
     * wor 2 if professional view)
     * 
     */
    void resetNumHorizontalSections();

    void resetButtonText();

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
      IsTriggered,
      TriggerLevel,
      SlopeButtonTriggered,
      IsAutoTriggered,
      RefreshTime,
      MuteOutput,
      Default
  };


  /**
   * Mapping from attachments' names to Attachments enum class.
   */
  const std::map<juce::String, ControlSection::Attachments> attachmentsMap{
    { "drawGrid", ControlSection::Attachments::DrawGrid },
    { "bufferLength", ControlSection::Attachments::BufferLength },
    { "isProfessional", ControlSection::Attachments::IsProfessional },
    { "isTriggered", ControlSection::Attachments::IsTriggered },
    { "triggerLevel", ControlSection::Attachments::TriggerLevel },
    { "slopeButtonTriggered", ControlSection::Attachments::SlopeButtonTriggered },
    { "autoTriggered", ControlSection::Attachments::IsAutoTriggered },
    { "refreshTime", ControlSection::Attachments::RefreshTime },
    { "muteOutput", ControlSection::Attachments::MuteOutput },

  };

  /**
   * Returns the proper attachment if a correct name is passed, Attachments::Default otherwise..
   * 
   * \param attachmentName Attachment name.
   * \return proper Attachments value.
   */ 
  Attachments resolveAttachment(juce::String attachmentName);  

  /**
   * Basic controls, always shown.
   * 
   */
  BasicControls basicControls;
  
  /**
   * Pro controls, shown only when ControlSection is expanded.
   * 
   */
  ProControls proControls;

  /**
   * Area where the oscilloscope toggle button is placed.
   * 
   */
  juce::Rectangle<int> toggleOscilloscopeArea; 

  // GUI expander elements
  juce::ToggleButton toggleOscilloscope;
  juce::Label toggleText;
  std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> isProfessionalAttachment;

  /**
   * Number of sections shown (1 or 2).
   * 
   */
  int numHorizontalSections;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlSection)

};
