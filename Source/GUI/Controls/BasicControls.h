/*
  ==============================================================================

    BasicControls.h
    Created: 16 Mar 2023 11:52:22am
    Author:  covariant

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BasicControls  : public juce::Component
{
public:
    BasicControls();
    ~BasicControls() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicControls)
};
