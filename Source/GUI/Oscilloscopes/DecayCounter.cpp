/*
  ==============================================================================

    DecayCounter.cpp
    Created: 23 Mar 2023 8:10:55pm
    Author:  covariant

  ==============================================================================
*/

#include "DecayCounter.h"

DecayCounter::DecayCounter(int framesPerSecond, int decaySeconds)
{
  
  this->framesPerSecond = framesPerSecond;
  this->decaySeconds = decaySeconds;
  this->decayTime = framesPerSecond * decaySeconds;
  this->maxDecayTime = OSCILLOSCOPE_MAX_DECAY_TIME();

}

DecayCounter::~DecayCounter()
{
}

bool DecayCounter::refreshNeeded()
{
  // decrement decayTime and redurn whether it hit 0
  if (decayTime-- > 0)
  {
    return false;
  }
  else
  {
    resetDecayTime();
    return true;
  }
}

void DecayCounter::resetDecayTime()
{

  decayTime = framesPerSecond * decaySeconds;

}

void DecayCounter::parameterChanged(const juce::String &parameterID, float newValue)
{

  decaySeconds = newValue * maxDecayTime;
  resetDecayTime();

}
