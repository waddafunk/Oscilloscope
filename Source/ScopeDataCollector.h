/*
  ==============================================================================

    ScopeDataCollector.h
    Created: 24 Feb 2023 6:49:28pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include "AudioBufferQueue.h"
#include <cstdlib>

namespace ranges = std::ranges;

/**
 * Class handling AudioBufferQueue.
 * 
 * Automatically fills a buffer, pushes it to the queue and starts filling the next one. 
 */
template <typename SampleType>
class ScopeDataCollector
{
public:
    //==============================================================================
    ScopeDataCollector(AudioBufferQueue<SampleType>& queueToUse)
        : audioBufferQueue(queueToUse)
    {
        buffer.resize(queueToUse.getBufferSize());
        bufferSize = buffer.size();
        numCollected = 0;
    }

    //==============================================================================
    /**
     * Processes incoming data.
     * 
     * \param data pointer to a series of data.
     * \param numSamples number of samples in the series.
     */
    void process(const SampleType* data, size_t numSamples)
    {
        size_t index = 0;

        int numBuffersToPush = numSamples / bufferSize;
        int numSamplesExceeding = numSamples % bufferSize;
        int numEmptyBuffers = 0;
        int additionalSamples = 0;

        if (state == State::waitingForTrigger)
        {
            std::fill(buffer.begin(), buffer.end(), 0);
            // If no sample over treshold push silence.
            if (std::all_of(data, data + numSamples, [](SampleType i) {return i < triggerLevel; }))
            {
                for (size_t i = 0; i < numBuffersToPush; i++)
                {
                    audioBufferQueue.push(buffer.data(), bufferSize);
                }
                numCollected = numSamplesExceeding;
            }
            // Else setup collecting stage
            else
            {
                auto result = ranges::find_if(data, data + numSamples, [](SampleType i) {return i > triggerLevel; });
                prevSample = *result;
                state = State::collecting;
                int firstSample = result - data;
                numEmptyBuffers = firstSample / bufferSize;
                additionalSamples = firstSample % bufferSize;
                for (size_t i = 0; i < numEmptyBuffers; i++)
                {
                    audioBufferQueue.push(buffer.data(), bufferSize);
                }
                numCollected = additionalSamples;
                index = firstSample;
            }
        }

        if (state == State::collecting)
        {
            while (index++ < numSamples)
            {
                buffer[numCollected++] = *data++;

                if (numCollected == buffer.size())
                {
                    audioBufferQueue.push(buffer.data(), buffer.size());
                    numCollected = 0;
                }
            }
        }
    }

private:
    //==============================================================================
    AudioBufferQueue<SampleType>& audioBufferQueue; /**< AudioBufferQueue */
    std::vector<SampleType> buffer; /**< Buffer to fill and push to audioBufferQueue */
    int bufferSize; /**< Size of @param buffer */
    size_t numCollected; /**< Number of samples collected. */
    SampleType prevSample = SampleType(100); /**< Last sample collected. */

    static constexpr auto triggerLevel = SampleType(0); /**< Level above which the oscilloscope starts drawing the waveform. */
    /**
     * States of the class.
     */
    enum class State { waitingForTrigger, collecting } state{ State::waitingForTrigger };
};