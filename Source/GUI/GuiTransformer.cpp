/*
  ==============================================================================

    GuiTransformer.cpp
    Created: 9 Mar 2023 9:40:25pm
    Author:  covariant

  ==============================================================================
*/

#include "GuiTransformer.h"

GuiTransformer::GuiTransformer(
    OscilloscopeAudioProcessor &aProcessor,
    float transitionDuration,
    std::function<void()> expandLambdaFunction,
    std::function<void()> contractLambdaFunction,
    std::function<void()> expandStartedLambdaFunction,
    std::function<void()> contractStartedLambdaFunction,
    std::function<void()> expandEndedLambdaFunction,
    std::function<void()> contractEndedLambdaFunction)
{
  // save variables
  this->transitionDuration = transitionDuration;
  isProfessional = aProcessor.getTreeState()->getParameterAsValue("isProfessional").getValue();
  aProcessor.getTreeState()->addParameterListener("isProfessional", this);
  framesRemaining = float(EDITOR_INITIAL_RATE()) * transitionDuration;

  // save lambdas
  expandLambda = expandLambdaFunction;
  expandStartedLambda = expandStartedLambdaFunction;
  expandEndedLambda = expandEndedLambdaFunction;
  contractLambda = contractLambdaFunction;
  contractStartedLambda = contractStartedLambdaFunction;
  contractEndedLambda = contractEndedLambdaFunction;
}

GuiTransformer::~GuiTransformer()
{
}

void GuiTransformer::timerCallback()
{
  // if animation not ended expand/contract
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
  // else trigger end lambda and stop
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
  // save new value
  isProfessional = bool(newValue);

  // trigger appropriate start lambda
  if (isProfessional)
  {

    expandStartedLambda();
  }
  else
  {

    contractStartedLambda();
  }

  // start timer
  startTimerHz(EDITOR_INITIAL_RATE());
}
