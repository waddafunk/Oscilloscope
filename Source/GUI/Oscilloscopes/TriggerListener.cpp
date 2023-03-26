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

  this->setTriggered = setTriggered;
  this->setUntriggered = setUntriggered;

}

void TriggerListener::parameterChanged(const juce::String &parameterID, float newValue)
{

  if (newValue)
  {
    setTriggered();
  }
  else
  {
    setUntriggered();
  }

}
