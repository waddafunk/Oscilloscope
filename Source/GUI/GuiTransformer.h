/**
 * \file GuiTransformer.h
 * \author Waddafunk (jacopo.piccirillo@gmail.com)
 * 
 * \version 0.2.0
 * \date 2023-03-09
 * 
 * \copyright Copyright (c) 2023
 * 
 */

#pragma once
#include "InitVariables.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"

/**
 * Class to handle GUI transition between Pro and
 * raw modalities.
 * 
 */
class GuiTransformer : public juce::Timer, private juce::AudioProcessorValueTreeState::Listener
{
public:

/**
 * Construct a new Gui Transformer object
 * 
 * \param transitionDuration Duration of the transition in seconds.
 * \param lambdaTriggers Vector containing lambda to trigger until 
 * @param framesRemaining is > 0 in first position and lamda to trigger
 * when it hits 0 in second poistion.
 */
  GuiTransformer(OscilloscopeAudioProcessor& aProcessor, int transitionDuration, std::array<std::function<void()>, 4> lambdaTriggers);
  ~GuiTransformer();

  private:

  /**
   * Frames of the transition remaining.
   * 
   */
  int framesRemaining;

  /**
   * Duration of the transition in seconds;
   * 
   */
  int transitionDuration;

  /**
   * Timer callback. Will be called @ frame rate for 
   * @param tframesRemaining frames.
   * 
   */
   void timerCallback() override;

   /**
   * Updates the buffer length when the parameter is modified.
   * 
   * \param parameterID Parameter ID (always "isProfessional").
   * \param newValue New value.
   */
   void parameterChanged(const juce::String& parameterID, float newValue) override;

   /**
    * Callback triggered when controlSection is expanding.
    * 
    */
   std::function<void()> expandLambda;
   /**
    * Callback triggered when controlSection is contracting.
    * 
    */
   std::function<void()> contractLambda;
   /**
    * Callback triggered when controlSection has finished expanding.
    * 
    */
   std::function<void()> expandEndedLambda;
   /**
    * Callback triggered when controlSection has finished contracting.
    * 
    */
   std::function<void()> contractEndedLambda;

   /**
    * Current callback to call.
    * 
    */
   std::function<void()>& currentLambda;
   /**
    * Current callback to call on animation end.
    * 
    */
   std::function<void()>& currentEndedLambda;
};


