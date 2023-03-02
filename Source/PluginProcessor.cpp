/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OscilloscopeAudioProcessor::OscilloscopeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), 
#endif
    processorTreeState(*this, nullptr, juce::Identifier("PARAMETERS"),
    { 
        std::make_unique<juce::AudioParameterBool>("drawGrid", "Draw Grid", false), 
        std::make_unique<juce::AudioParameterInt>("bufferLength", "Scope Length", 2000, 22050, 5000),
    })
{

    size_t bufferSize = 5000;
    audioBufferQueue.reset(new AudioBufferQueue<float>(44100, getEditorRefreshRate()));
    scopeDataCollector.reset(new ScopeDataCollector(*audioBufferQueue.get()));
}

OscilloscopeAudioProcessor::~OscilloscopeAudioProcessor()
{
}

//==============================================================================
const juce::String OscilloscopeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OscilloscopeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OscilloscopeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OscilloscopeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OscilloscopeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OscilloscopeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OscilloscopeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OscilloscopeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OscilloscopeAudioProcessor::getProgramName (int index)
{
    return {};
}

void OscilloscopeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OscilloscopeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    this->sampleRate = sampleRate;
    audioBufferQueue.reset(new AudioBufferQueue<float>(sampleRate, getEditorRefreshRate()));
    scopeDataCollector.reset(new ScopeDataCollector(*audioBufferQueue.get()));
}

void OscilloscopeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OscilloscopeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void OscilloscopeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    scopeDataCollector->process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
}

//==============================================================================
bool OscilloscopeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OscilloscopeAudioProcessor::createEditor()
{
    return new OscilloscopeAudioProcessorEditor (*this);
}

//==============================================================================
void OscilloscopeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // Serialize the XmlElement object to a MemoryBlock
    copyXmlToBinary(*processorTreeState.state.createXml().get(), destData);
}

void OscilloscopeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // Create an XmlElement object to hold the state
    // Deserialize the XmlElement object from the MemoryBlock
    // Deserialize the binary data into an XmlElement
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
    {
        // Get the child element of the XmlElement and create a ValueTree from it
        juce::ValueTree stateTree = juce::ValueTree::fromXml(*xmlState);

        // Restore the state of the AudioProcessorValueTreeState object from the ValueTree
        processorTreeState.replaceState(stateTree);
    }
    xmlState.reset();
    
}

int OscilloscopeAudioProcessor::getSampleRate()
{
    return this->sampleRate;
}

AudioBufferQueue<float>* OscilloscopeAudioProcessor::getAudioBufferQueue()
{
    return this->audioBufferQueue.get();
}

juce::AudioProcessorValueTreeState* OscilloscopeAudioProcessor::getTreeState()
{
    return &this->processorTreeState;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OscilloscopeAudioProcessor();
}

int OscilloscopeAudioProcessor::getEditorWidth()
{
    auto size = processorTreeState.state.getOrCreateChildWithName("lastSize", nullptr);
    return size.getProperty("width", EDITOR_INITIAL_WIDTH);
}
int OscilloscopeAudioProcessor::getEditorHeight()
{
    auto size = processorTreeState.state.getOrCreateChildWithName("lastSize", nullptr);
    return size.getProperty("height", EDITOR_INITIAL_HEIGHT);
}

void OscilloscopeAudioProcessor::storeEditorSize(int width, int height)
{
    auto size = processorTreeState.state.getOrCreateChildWithName("lastSize", nullptr);
    size.setProperty("width", width, nullptr);
    size.setProperty("height", height, nullptr);
}

int OscilloscopeAudioProcessor::getEditorRefreshRate()
{
    auto rate = processorTreeState.state.getOrCreateChildWithName("editorRefreshRate", nullptr);
    return rate.getProperty("height", EDITOR_INITIAL_RATE);
}
