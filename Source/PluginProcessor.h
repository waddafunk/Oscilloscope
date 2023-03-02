/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ScopeDataCollector.h"
#include "AudioBufferQueue.h"

constexpr int EDITOR_INITIAL_WIDTH = 1400;
constexpr int EDITOR_INITIAL_HEIGHT = 700;
constexpr int EDITOR_INITIAL_RATE = 30;

//==============================================================================
/**
*/
class OscilloscopeAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    OscilloscopeAudioProcessor();
    ~OscilloscopeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    int getSampleRate();

    AudioBufferQueue<float>* getAudioBufferQueue();

    juce::AudioProcessorValueTreeState* getTreeState();

    /**
     * Get editor width.
     * 
     * \return Editor width.
     */
    int getEditorWidth();
    /**
     * Get editor height.
     * 
     * \return Editor height.
     */
    int getEditorHeight();
    /**
     * Store editor size in order to be able to recover it from the @param processorTreeState ValueTree.
     * This is done in order to make the editor size persistent.
     * 
     * \param width Editor width.
     * \param height Editor height.
     */
    void storeEditorSize(int width, int height);

    /**
     * Get the stored refresh rate.
     * 
     * \return Stored editors' refresh rate.
     */
    int getEditorRefreshRate();

private:
    void parameterChanged(const juce::String& parameterID, float newValue);
    //==============================================================================
    std::unique_ptr<AudioBufferQueue<float>> audioBufferQueue;
    std::unique_ptr <ScopeDataCollector<float>> scopeDataCollector;
    juce::AudioProcessorValueTreeState processorTreeState;
    int sampleRate;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscilloscopeAudioProcessor)
};
