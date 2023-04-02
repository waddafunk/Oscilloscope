/*
  ==============================================================================

    ColorPalette.h
    Created: 8 Mar 2023 12:48:40pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/**
 * Control section colour.
 *
 * \return juce::Colour Control section colour.
 */
static juce::Colour CONTROLSECTIONCOLOR() { return juce::Colour(3, 187, 133); };

/**
 * Button to expand GUI colour.
 *
 * \return juce::Colour Button to expand GUI colour.
 */
static juce::Colour GUITOGGLERCOLOUR() { return juce::Colours::ghostwhite.withAlpha(float(0.4)); };

/**
 * Waveform colour.
 *
 * \return juce::Colour Waveform colour.
 */
static juce::Colour WAVEFORMCOLOUR() { return juce::Colours::beige; };
