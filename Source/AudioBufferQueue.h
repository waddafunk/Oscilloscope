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
    static constexpr size_t order = 9; /**< Order of the buffer size */
    static constexpr size_t bufferSize = 1U << order; /**< Buffer size */
    static constexpr size_t numBuffers = 5; /**< Number of buffers */

    //==============================================================================
    /**
     * Pushes a buffer of data.
     * 
     * \param dataToPush Data to push to the buffers
     * \param numSamples Number of samples
     */
    void push(const SampleType* dataToPush, size_t numSamples)
    {
        jassert(numSamples <= bufferSize);

        int start1, size1, start2, size2;
        abstractFifo.prepareToWrite(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        if (size1 > 0)
            juce::FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, (int)juce::jmin(bufferSize, numSamples));

        abstractFifo.finishedWrite(size1);
    }

    //==============================================================================
    /**
     * Pops a buffer of data.
     * 
     * \param outputBuffer output buffer
     */
    void pop(SampleType* outputBuffer)
    {
        int start1, size1, start2, size2;
        abstractFifo.prepareToRead(1, start1, size1, start2, size2);

        jassert(size1 <= 1);
        jassert(size2 == 0);

        if (size1 > 0)
            juce::FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)bufferSize);

        abstractFifo.finishedRead(size1);
    }

private:
    //==============================================================================
    juce::AbstractFifo abstractFifo{ numBuffers }; /**<a href = "https://docs.juce.com/master/classAbstractFifo.html">Abstract Fifo< / a> */
    std::array<std::array<SampleType, bufferSize>, numBuffers> buffers; /**< Array of buffers */
};