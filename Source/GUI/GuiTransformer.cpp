/*
  ==============================================================================

    GuiTransformer.cpp
    Created: 9 Mar 2023 9:40:25pm
    Author:  covariant

  ==============================================================================
*/

#include "GuiTransformer.h"

GuiTransformer::GuiTransformer(
   OscilloscopeAudioProcessor& aProcessor, 
   float transitionDuration, 
   std::function<void()> expandLambdaFunction,
   std::function<void()> contractLambdaFunction,
   std::function<void()> expandEndedLambdaFunction,
   std::function<void()> contractEndedLambdaFunction
  )
{
  this->transitionDuration = transitionDuration;

  isProfessional = false;

  aProcessor.getTreeState()->addParameterListener("isProfessional", this);

  framesRemaining = float(EDITOR_INITIAL_RATE()) * transitionDuration;
  
  expandLambda = expandLambdaFunction; 
  expandEndedLambda = expandEndedLambdaFunction;
  contractLambda = contractLambdaFunction;
  contractEndedLambda = contractEndedLambdaFunction;

}

GuiTransformer::~GuiTransformer()
{
}

void GuiTransformer::timerCallback()
{
  if (framesRemaining-- > 0)
  {
    if (isProfessional)
    {
      expandLambda();
    }
    else
    {
      contractLambda();
    }
  }
  else
  {
    if (isProfessional)
    {
      expandEndedLambda();
    }
    else
    {
      contractEndedLambda();
    }
    framesRemaining = float(EDITOR_INITIAL_RATE()) * transitionDuration;
    stopTimer();
  }
}

void GuiTransformer::parameterChanged(const juce::String &parameterID, float newValue)
{
  isProfessional = bool(newValue);
  startTimerHz(EDITOR_INITIAL_RATE());
}
