/*
  ==============================================================================

    GuiTransformer.cpp
    Created: 9 Mar 2023 9:40:25pm
    Author:  covariant

  ==============================================================================
*/

#include "GuiTransformer.h"

GuiTransformer::GuiTransformer(OscilloscopeAudioProcessor& aProcessor, int transitionDuration, std::array<std::function<void()>, 4> lambdaTriggers)
: currentLambda(lambdaTriggers[0]), currentEndedLambda(lambdaTriggers[1])
{
  this->transitionDuration = transitionDuration;

  aProcessor.getTreeState()->addParameterListener("isProfessional", this);

  framesRemaining = EDITOR_INITIAL_RATE() * transitionDuration;
  
  expandLambda = lambdaTriggers[0]; 
  expandEndedLambda = lambdaTriggers[1];
  contractLambda = lambdaTriggers[2];
  contractEndedLambda = lambdaTriggers[3];

}

GuiTransformer::~GuiTransformer()
{
}

void GuiTransformer::timerCallback()
{
  if (framesRemaining-- > 0)
  {
    currentLambda();
  }
  else
  {
    currentEndedLambda();
    framesRemaining = EDITOR_INITIAL_RATE() * transitionDuration;
    stopTimer();
  }
}

void GuiTransformer::parameterChanged(const juce::String &parameterID, float newValue)
{
  bool isProfessional = bool(newValue);

  if (isProfessional)
  {
    currentLambda = expandLambda;
    currentEndedLambda = expandEndedLambda;
  }
  else
  {
    currentLambda = contractLambda;
    currentEndedLambda = contractEndedLambda;
  }

  startTimerHz(EDITOR_INITIAL_RATE());
}
