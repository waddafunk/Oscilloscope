/*
  ==============================================================================

    AudioBufferQueue.h
    Created: 24 Feb 2023 6:49:10pm
    Author:  wadda

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
/**
 * A Fifo queue of buffers working with data of SampleType type.
 */
template <typename SampleType>
class AudioBufferQueue
{
public:
    //==============================================================================

    /**
     * Constructor.
     *
     * \param newBufferSize Buffer size.
     */
    AudioBufferQueue(int hostRate = 44100, int editorFramesPerSecond = 30, int numSamplesPerBlock = 441)
    {
        // set number of samples per frame
        this->bufferSize = hostRate / editorFramesPerSecond;

        // calculate capacitz and reset buffers & Abstract FIFO
        int capacity = numSamplesPerBlock * 2;
        numBuffers = std::max((int)(capacity / bufferSize + 1), (int)5);
        abstractFifo.reset(new juce::AbstractFifo(numBuffers));
        buffers.resize(numBuffers);
        for (size_t i = 0; i < numBuffers; i++)
        {
            buffers[i].resize(bufferSize);
        }
    }

    /**
     * Pushes a buffer of data.
     *
     * \param dataToPush Data to push to the buffers
     * \param numSamples Number of samples
     */
    void push(const SampleType *dataToPush, size_t numSamples)
    {
        // no more samples than buffer size
        jassert(numSamples <= bufferSize);

        // get pointer to where to write
        int start1, size1 = 0, start2, size2;
        abstractFifo->prepareToWrite(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        // write data
        if (size1 > 0)
        {
            juce::FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, numSamples);
        }

        abstractFifo->finishedWrite(size1);
    }

    //==============================================================================
    /**
     * Pops a buffer of data.
     *
     * \param outputBuffer output buffer
     */
    void pop(SampleType *outputBuffer)
    {
        // get where to read
        int start1, size1, start2, size2;
        abstractFifo->prepareToRead(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        // read
        if (size1 > 0)
            juce::FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)buffers[(size_t)start1].size());

        abstractFifo->finishedRead(size1);
    }

    /**
     * Empties all buffers.
     *
     */
    void flush()
    {
        // fill every buffer with 0
        for (auto i = 0; i < numBuffers; i++)
        {
            std::fill(std::begin(buffers[i]), std::end(buffers[i]), 0);
        }
    }

    /**
     * Get buffer size.
     *
     * \return Buffer size.
     */
    size_t getBufferSize()
    {
        return bufferSize;
    }

private:
    //==============================================================================
    size_t numBuffers = 5;                            /**< Number of buffers */
    std::unique_ptr<juce::AbstractFifo> abstractFifo; /**<a href = "https://docs.juce.com/master/classAbstractFifo.html">Abstract Fifo< / a> */
    std::vector<std::vector<SampleType>> buffers;     /**< Array of buffers */
    size_t bufferSize;                                /**< Buffer size */
};