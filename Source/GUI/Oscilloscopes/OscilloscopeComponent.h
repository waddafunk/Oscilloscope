/*
  ==============================================================================

    OscilloscopeComponent.h
    Created: 24 Feb 2023 6:47:52pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "AudioBufferQueue.h"
#include "PluginProcessor.h"
# include "InitVariables.h"

/**
 * Oscilloscope graphical component.
 * 
 * Inherits from <a href="https://docs.juce.com/master/classComponent.html">JUCE Component</a> 
 * and <a href="https://docs.juce.com/master/classTimer.html">JUCE timer</a> 
 */
class OscilloscopeComponent : public juce::Component, public juce::AudioProcessorValueTreeState::Listener,
    private juce::Timer
{
public:

    //==============================================================================
    /**
     * Constructor.
     * 
     * \param queueToUse AudioBufferQueue to use
     */
    OscilloscopeComponent(OscilloscopeAudioProcessor& aProcessor, int sampleRate, int framesPerSecond);

    /**
     * Destroy the Oscilloscope Component object.
     * 
     */
    ~OscilloscopeComponent();

    //==============================================================================
    /**
     * Sets frames per seconds.
     * 
     * \param framesPerSecond Frames per second
     */
    void setFramesPerSecond(int framesPerSecond);

    //==============================================================================

    /**
     * Draws oscilloscope's grid.
     * 
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a>. 
     * \param w width.
     * \param h heigth.
     */
    void drawGrid(juce::Graphics& g, float w, float h);

    /**
     * Paints the component.
     * 
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a>. 
     */
    void paint(juce::Graphics& g) override;

    //==============================================================================
    /**
     * Called when component is resized.
     * 
     */
    void resized() override;

protected:
  int sampleRate; /**< Sample rate */
  std::vector<float> sampleData; /**< Data currently displayed */
  OscilloscopeAudioProcessor& audioProcessor;

private:
    //==============================================================================
    int displayLength;
    std::vector<float> newlyPopped; /**< Last popped array */
    std::vector<float> notInterpolatedData; /** < Raw new data*/
    std::vector<float> newData; /** < Interpolated new data*/
    double ratio = 1.;
    juce::Interpolators::Linear interpolator;

    /**
     * Updates the buffer length when the parameter is modified.
     * 
     * \param parameterID Parameter ID (always "bufferLength).
     * \param newValue New value.
     */
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    //==============================================================================
    /**
     * Timer callback.
     * 
     * @see <a href="https://docs.juce.com/master/classTimer.html">JUCE Timer </a>
     * 
     */
    void timerCallback() override;

    //==============================================================================
    /**
     * Plots the waveform. This method is implemented in the subclasses in order to
     * avoid having a check on the selected draw modality of the waveform every time 
     * it is plotted. 
     * 
     * \param data Samples to plot
     * \param numSamples Number of samples
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a> 
     * \param rect <a href="https://docs.juce.com/master/classRectangle.html">JUCE Rectangle </a> 
     * \param scaler Scale factor.
     * \param offset Y-axis offset.
     */
    virtual void plot(
        juce::Graphics& g,
        juce::Rectangle<float> rect,
        float scaler = float(1),
        float offset = float(0)) = 0;

    /**
     * This callback will be called in @ref timerCallback and is left
     * to be implemented in the subclasses to handle subclass-specific
     * operations. 
     * 
     */
    virtual void subclassSpecificCallback() = 0;
};