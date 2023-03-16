/*
  ==============================================================================

    OscilloscopeComponent.cpp
    Created: 24 Feb 2023 6:47:52pm
    Author:  wadda

  ==============================================================================
*/

#include "OscilloscopeComponent.h"

OscilloscopeComponent::OscilloscopeComponent(OscilloscopeAudioProcessor& aProcessor, int sampleRate, int framesPerSecond) :
    audioProcessor(aProcessor)
{
    setFramesPerSecond(framesPerSecond);
    this->sampleRate = sampleRate;
    displayLength = (int)(*aProcessor.getTreeState()->getRawParameterValue("bufferLength") * aProcessor.getSampleRate());
    ratio = (double)displayLength / (double)EDITOR_INITIAL_WIDTH();
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
    interpolator.reset();
}

void OscilloscopeComponent::setFramesPerSecond(int framesPerSecond)
{
    jassert(framesPerSecond > 0 && framesPerSecond < 1000);
    startTimerHz(framesPerSecond);
}

void OscilloscopeComponent::drawGrid(juce::Graphics& g, float w, float h)
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
    float duration = *audioProcessor.getTreeState()->getRawParameterValue("bufferLength") * static_cast<float>(10000);
    duration /= 10;
    auto xText = juce::String(duration, 2);
    xText.append(" ms", 3);

    g.drawLine(w - 95, h - 39, w - 95, h - 39 - fontHeight);
    g.drawLine(w - 85, h - 39 - fontHeight / 2, w - 95, h - 39 - fontHeight / 2);
    g.drawLine(w - 85, h - 39, w - 85, h - 39 - fontHeight);
    g.drawSingleLineText(xText, w - 80, h - 39);

    auto yText = "0.1";
    g.drawLine(w - 95, h - 19, w - 85, h - 19);
    g.drawLine(w - 90, h - 19 - fontHeight, w - 90, h - 19);
    g.drawLine(w - 95, h - 19 - fontHeight, w - 85, h - 19 - fontHeight);
    g.drawSingleLineText(yText, w - 80, h - 19);
}

void OscilloscopeComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    auto area = getLocalBounds();
    auto h = (float)area.getHeight();
    auto w = (float)area.getWidth();

    if (audioProcessor.getTreeState()->getParameterAsValue("drawGrid").getValue())
    {
        drawGrid(g, w, h);
    }

    // Oscilloscope
    auto scopeRect = juce::Rectangle<float>{ float(0), float(0), w, h };
    plot(sampleData.data(), sampleData.size(), g, scopeRect, float(1), h / 2);

}

void OscilloscopeComponent::resized()
{
}

void OscilloscopeComponent::parameterChanged(const juce::String& parameterID, float newValue)
{
    ratio = newValue * audioProcessor.getSampleRate() / EDITOR_INITIAL_WIDTH();
    displayLength = newValue * audioProcessor.getSampleRate() / ratio;
    sampleData.resize(displayLength);
    newData.resize(sampleData.size());
    int queueSize = audioProcessor.getAudioBufferQueue()->getBufferSize();
    double dataLength = queueSize / ratio;
    newlyPopped.resize(dataLength);
}

void OscilloscopeComponent::timerCallback()
{
    audioProcessor.getAudioBufferQueue()->pop(notInterpolatedData.data());
    int queueSize = newlyPopped.size();
    interpolator.process(ratio, notInterpolatedData.data(), newlyPopped.data(), queueSize);
    std::copy(sampleData.data() + queueSize, sampleData.data() + sampleData.size(), newData.begin());
    std::copy(newlyPopped.data(), newlyPopped.data() + queueSize, newData.begin() + sampleData.size() - queueSize);
    sampleData = newData;
    repaint();
}
