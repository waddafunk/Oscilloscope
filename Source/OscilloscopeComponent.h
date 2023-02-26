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
    OscilloscopeComponent(Queue& queueToUse, int sampleRate)
        : audioBufferQueue(queueToUse)
    {
        sampleData.fill(SampleType(0));
        setFramesPerSecond(30);
        this->sampleRate = sampleRate;
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
     * Set to true to draw grid, false otherwise.
     * 
     * \param newValue value to set.
     */
    void setGridCheck(bool newValue)
    {
        this->gridCheck = newValue;
    }

    /**
     * Draws oscilloscope's grid.
     * 
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a>. 
     * \param w width.
     * \param h heigth.
     */
    void drawGrid(juce::Graphics& g, SampleType w, SampleType h)
    {
        g.drawLine(0, h / 2, w, h / 2);
        g.drawLine(1, 0, 1, h);

        for (size_t i = 0; i < 10; i++)
        {
            float xPos = i * w / 10;
            float yPos = i * h / 10;
            g.drawLine(xPos, h / 2 - 5, xPos, h / 2 + 5);
            g.drawLine(0, yPos, 5, yPos);
        }

        float fontHeight = g.getCurrentFont().getAscent();
        float duration = sampleData.size() * 1000 / sampleRate;
        duration /= 10;
        auto xText = juce::String(duration);
        xText.append(" ms", 3);

        g.drawLine(w - 65, h - 39, w - 65, h - 39 - fontHeight);
        g.drawLine(w - 55, h - 39 - fontHeight / 2, w - 65, h - 39 - fontHeight / 2);
        g.drawLine(w - 55, h - 39, w - 55, h - 39 - fontHeight);
        g.drawSingleLineText(xText, w - 50, h - 39);

        auto yText = "0.1";
        g.drawLine(w - 65, h - 19, w - 55, h - 19);
        g.drawLine(w - 60, h - 19 - fontHeight, w - 60, h - 19);
        g.drawLine(w - 65, h - 19 - fontHeight, w - 55, h - 19 - fontHeight);
        g.drawSingleLineText(yText, w - 50, h - 19);
    }

    /**
     * Paints the component.
     * 
     * \param g <a href="https://docs.juce.com/master/classGraphics.html">JUCE Graphics </a>. 
     */
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);

        auto area = getLocalBounds();
        auto h = (SampleType)area.getHeight();
        auto w = (SampleType)area.getWidth();

        if (gridCheck)
        {
            drawGrid(g, w, h);
        }
     
        // Oscilloscope
        auto scopeRect = juce::Rectangle<SampleType>{ SampleType(0), SampleType(0), w, h };
        plot(sampleData.data(), sampleData.size(), g, scopeRect, SampleType(1), h / 2);

        g.setColour(juce::Colours::dimgrey);
        auto contour = juce::Line<float>(0, h, w, h);
        g.drawLine(contour, 8.0f);

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
    int sampleRate; /**< Sample rate */
    bool gridCheck = false;

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