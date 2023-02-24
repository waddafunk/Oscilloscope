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

/**
 * Oscilloscope graphical component for SampleType type data.
 * 
 * Inherits from <a href="https://docs.juce.com/master/classComponent.html">JUCE Component</a> and <a href="https://docs.juce.com/master/classTimer.html">JUCE timer</a> 
 */
template <typename SampleType>
class OscilloscopeComponent : public juce::Component,
    private juce::Timer
{
public:
    using Queue = AudioBufferQueue<SampleType>;

    //==============================================================================
    /**
     * Constructor.
     * 
     * \param queueToUse AudioBufferQueue to use
     */
    OscilloscopeComponent(Queue& queueToUse)
        : audioBufferQueue(queueToUse)
    {
        sampleData.fill(SampleType(0));
        setFramesPerSecond(30);
    }

    //==============================================================================
    /**
     * Sets frames per seconds.
     * 
     * \param framesPerSecond Frames per second
     */
    void setFramesPerSecond(int framesPerSecond)
    {
        jassert(framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz(framesPerSecond);
    }

    //==============================================================================
    /**
     * Paints the component.
     * 
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a> 
     */
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);

        auto area = getLocalBounds();
        auto h = (SampleType)area.getHeight();
        auto w = (SampleType)area.getWidth();

        // Oscilloscope
        auto scopeRect = juce::Rectangle<SampleType>{ SampleType(0), SampleType(0), w, h };
        plot(sampleData.data(), sampleData.size(), g, scopeRect, SampleType(1), h / 2);

    }

    //==============================================================================
    /**
     * Called when component is resized.
     * 
     */
    void resized() override {}

private:
    //==============================================================================
    Queue& audioBufferQueue; /**< AudioBufferQueue */
    std::array<SampleType, Queue::bufferSize> sampleData; /**< Data currently displayed */


    //==============================================================================
    /**
     * Timer callback.
     * 
     * @see <a href="https://docs.juce.com/master/classTimer.html">JUCE Timer </a>
     * 
     */
    void timerCallback() override
    {
        audioBufferQueue.pop(sampleData.data());
        repaint();
    }

    //==============================================================================
    /**
     * Plots the waveform.
     * 
     * \param data Samples to plot
     * \param numSamples Number of samples
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a> 
     * \param rect <a href="https://docs.juce.com/master/classRectangle.html">JUCE Rectangle </a> 
     * \param scaler Scale factor.
     * \param offset Y-axis offset.
     */
    static void plot(const SampleType* data,
        size_t numSamples,
        juce::Graphics& g,
        juce::Rectangle<SampleType> rect,
        SampleType scaler = SampleType(1),
        SampleType offset = SampleType(0))
    {
        auto w = rect.getWidth();
        auto h = rect.getHeight();
        auto right = rect.getRight();

        auto center = rect.getBottom() - offset;
        auto gain = h * scaler;

        g.setColour(juce::Colours::beige);

        for (size_t i = 1; i < numSamples; ++i)
            g.drawLine({ juce::jmap(SampleType(i - 1), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right)),
                          center - gain * data[i - 1],
                          juce::jmap(SampleType(i), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right)),
                          center - gain * data[i] });
    }
};