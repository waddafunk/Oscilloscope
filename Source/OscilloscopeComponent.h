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
 * Oscilloscope graphical component for SampleType type data.
 * 
 * Inherits from <a href="https://docs.juce.com/master/classComponent.html">JUCE Component</a> and <a href="https://docs.juce.com/master/classTimer.html">JUCE timer</a> 
 */
class OscilloscopeComponent : public juce::Component, private juce::AudioProcessorValueTreeState::Listener,
    private juce::Timer
{
public:

    //==============================================================================
    /**
     * Constructor.
     * 
     * \param queueToUse AudioBufferQueue to use
     */
    OscilloscopeComponent(OscilloscopeAudioProcessor& aProcessor, int sampleRate, int framesPerSecond) :
        audioProcessor(aProcessor)
    {
        setFramesPerSecond(framesPerSecond);
        this->sampleRate = sampleRate;
        displayLength = *aProcessor.getTreeState()->getRawParameterValue("bufferLength");
        ratio = displayLength / EDITOR_INITIAL_WIDTH();
        displayLength /= ratio;
        double dataLength = audioProcessor.getAudioBufferQueue()->getBufferSize() / ratio;
        sampleData.resize(EDITOR_INITIAL_WIDTH());
        newData.resize(sampleData.size());
        newlyPopped.resize(dataLength);
        notInterpolatedData.resize(audioProcessor.getAudioBufferQueue()->getBufferSize());
        std::fill(sampleData.begin(), sampleData.end(), 0);
        std::fill(newlyPopped.begin(), newlyPopped.end(), 0);
        std::fill(notInterpolatedData.begin(), notInterpolatedData.end(), 0);
        std::fill(newData.begin(), newData.end(), 0);
        aProcessor.getTreeState()->addParameterListener("bufferLength", this);
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
    void drawGrid(juce::Graphics& g, float w, float h)
    {
        g.setColour(juce::Colours::ghostwhite);
        g.setOpacity(0.8);
        g.drawLine(0, h / 2, w, h / 2);
        g.drawLine(1, 0, 1, h);

        for (size_t i = 0; i < 10; i++)
        {
            float xPos = i * w / 10;
            float yPos = i * h / 10;
            g.drawLine(xPos, h / 2 - 8, xPos, h / 2 + 8);
            g.drawLine(0, yPos, 8, yPos);
        }

        float fontHeight = g.getCurrentFont().getAscent();
        float duration = *audioProcessor.getTreeState()->getRawParameterValue("bufferLength") * static_cast<float>(1000) / sampleRate;
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
        auto h = (float)area.getHeight();
        auto w = (float)area.getWidth();

        if (gridCheck)
        {
            drawGrid(g, w, h);
        }
     
        // Oscilloscope
        auto scopeRect = juce::Rectangle<float>{ float(0), float(0), w, h };
        plot(sampleData.data(), sampleData.size(), g, scopeRect, float(1), h / 2);

        g.setColour(juce::Colours::dimgrey);
        auto contour = juce::Line<float>(0, h, w, h);
        g.drawLine(contour, 8.0f);

    }

    //==============================================================================
    /**
     * Called when component is resized.
     * 
     */
    void resized() override { };


private:
    //==============================================================================
    OscilloscopeAudioProcessor& audioProcessor;
    int displayLength;
    std::vector<float> sampleData; /**< Data currently displayed */
    std::vector<float> newlyPopped; /**< Last popped array */
    std::vector<float> notInterpolatedData;
    std::vector<float> newData;
    int sampleRate; /**< Sample rate */
    bool gridCheck = false;
    int ratio = 1;
    juce::Interpolators::Linear interpolator;

    /**
     * Updates the buffer length when the parameter is modified.
     * 
     * \param parameterID Parameter ID (always "bufferLength).
     * \param newValue New value.
     */
    void parameterChanged(const juce::String& parameterID, float newValue) override
    {
        ratio = newValue / EDITOR_INITIAL_WIDTH();
        displayLength = newValue / ratio;
        sampleData.resize(displayLength);
        newData.resize(sampleData.size());
        int queueSize = audioProcessor.getAudioBufferQueue()->getBufferSize();
        double dataLength = queueSize / ratio;
        newlyPopped.resize(dataLength);
    }

    //==============================================================================
    /**
     * Timer callback.
     * 
     * @see <a href="https://docs.juce.com/master/classTimer.html">JUCE Timer </a>
     * 
     */
    void timerCallback() override
    {
        audioProcessor.getAudioBufferQueue()->pop(notInterpolatedData.data());
        interpolator.reset();
        int queueSize = newlyPopped.size();
        interpolator.process(ratio, notInterpolatedData.data(), newlyPopped.data(), queueSize);
        std::copy(sampleData.data() + queueSize, sampleData.data() + sampleData.size(), newData.begin());
        std::copy(newlyPopped.data(), newlyPopped.data() + queueSize, newData.begin() + sampleData.size() - queueSize);
        sampleData = newData;
        repaint();
    }

protected:
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
    virtual void plot(const float* data,
        size_t numSamples,
        juce::Graphics& g,
        juce::Rectangle<float> rect,
        float scaler = float(1),
        float offset = float(0)) = 0;
};