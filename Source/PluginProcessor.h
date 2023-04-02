/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "InitVariables.h"
#include "ScopeDataCollector.h"
#include "AudioBufferQueue.h"

//==============================================================================
/**
 * \brief Oscilloscope audio processor.
 *
 * This class handles all the audio I/O operations, initialization and state
 * retrieval.
 *
 */
class OscilloscopeAudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    ,
                                   public juce::AudioProcessorARAExtension
#endif
{
public:
  //==============================================================================
  /**
   * Construct a new Oscilloscope Audio Processor object
   *
   */
  OscilloscopeAudioProcessor();
  /**
   * Destroy the Oscilloscope Audio Processor object
   *
   */
  ~OscilloscopeAudioProcessor() override;

  //==============================================================================
  /**
   * Use this method as the place to do any pre-playback
   * initialisation.
   *
   * \param sampleRate Sample rate.
   * \param samplesPerBlock Samples per block.
   */
  void prepareToPlay(double sampleRate, int samplesPerBlock) override;

  /**
   * When playback stops, you can use this as an opportunity to free up any
   * spare memory, etc.
   */
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

  /**
   * Process a block of audio data.
   *
   */
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  //==============================================================================
  /**
   * Create a Editor object
   *
   * \return juce::AudioProcessorEditor*
   */
  juce::AudioProcessorEditor *createEditor() override;

  /**
   * Return whether the plugin has an editor.
   *
   * \return true if the plugin has an editor.
   * \return false otherwise.
   */
  bool hasEditor() const override;

  //==============================================================================
  /**
   * Get the Name object
   *
   * \return const juce::String
   */
  const juce::String getName() const override;

  /**
   * Returns whether the plugin accepts midi.
   *
   * \return true if the plugin accepts midi.
   * \return false otherwise.
   */
  bool acceptsMidi() const override;

  /**
   * Returns whether the plugin produces midi.
   *
   * \return true if the plugin produces midi.
   * \return false otherwise.
   */
  bool producesMidi() const override;

  /**
   * Return whether the plugin is a midi effect.
   *
   * \return true if the plugin is a midi effect.
   * \return false otherwise.
   */
  bool isMidiEffect() const override;

  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String &newName) override;

  //==============================================================================
  /**
   * Get the State Information object
   *
   * \param destData memory block where to store information.
   */
  void getStateInformation(juce::MemoryBlock &destData) override;

  /**
   * Set the State Information object
   *
   * \param data memory block where to read information.
   * \param sizeInBytes size in bytes.
   */
  void setStateInformation(const void *data, int sizeInBytes) override;

  /**
   * Get the Sample Rate of the host.
   *
   * \return int sample rate.
   */
  int getSampleRate();

  /**
   * Get the AudioBufferQueue object
   *
   * \return AudioBufferQueue<float>* Audio Buffer Queue object.
   */
  AudioBufferQueue<float> *getAudioBufferQueue();

  /**
   * Get the TreeState object
   *
   * \return juce::AudioProcessorValueTreeState* TreeState.
   */
  juce::AudioProcessorValueTreeState *getTreeState();

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
  /**
   * Reset all value tree parameters to default.
   *
   * \param tree  ValueTree
   */
  void resetAllValuesToDefault(juce::ValueTree tree);

  //==============================================================================
  std::unique_ptr<AudioBufferQueue<float>> audioBufferQueue;
  std::unique_ptr<ScopeDataCollector<float>> scopeDataCollector;
  juce::AudioProcessorValueTreeState processorTreeState;
  int sampleRate = 44100;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscilloscopeAudioProcessor)
};
