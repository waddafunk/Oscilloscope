/*
  ==============================================================================

    TriggerListener.cpp
    Created: 26 Mar 2023 2:06:54pm
    Author:  covariant

  ==============================================================================
*/

#include "TriggerListener.h"

TriggerListener::TriggerListener(std::function<void()> setTriggered, std::function<void()> setUntriggered)
{
  // set lambdas
  this->setTriggered = setTriggered;
  this->setUntriggered = setUntriggered;
}

void TriggerListener::parameterChanged(const juce::String &parameterID, float newValue)
{
  // if triggered mode
  if (newValue)
  {
    setTriggered();
  }
  // if untriggered mode
  else
  {
    setUntriggered();
  }
}
