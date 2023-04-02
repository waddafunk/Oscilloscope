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
    * \param aProcessor PluginProcessor reference.
    * \param transitionDuration Duration of the transition in seconds.
    * \param expandLambdaFunction Lambda function to call when expanding ControlSection.
    * \param contractLambdaFunction Lambda function to call when contracting ControlSection.
    * \param expandStartedLambdaFunction Lambda function to call when ControlSection expansion starts.
    * \param contractStartedLambdaFunction Lambda function to call when ControlSection contraction starts.
    * \param expandEndedLambdaFunction Lambda function to call when ControlSection expansion ends.
    * \param contractEndedLambdaFunction Lambda function to call when ControlSection contraction ends.
    */
   GuiTransformer(
       OscilloscopeAudioProcessor &aProcessor,
       float transitionDuration,
       std::function<void()> expandLambdaFunction,
       std::function<void()> contractLambdaFunction,
       std::function<void()> expandStartedLambdaFunction,
       std::function<void()> contractStartedLambdaFunction,
       std::function<void()> expandEndedLambdaFunction,
       std::function<void()> contractEndedLambdaFunction);
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
   float transitionDuration;

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
   void parameterChanged(const juce::String &parameterID, float newValue) override;

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
    * Callback triggered when controlSection has started expanding.
    *
    */
   std::function<void()> expandStartedLambda;
   /**
    * Callback triggered when controlSection has started contracting.
    *
    */
   std::function<void()> contractStartedLambda;
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
    * Boolean storing whether the ControlSection is professional
    * or not.
    *
    */
   bool isProfessional;
};
