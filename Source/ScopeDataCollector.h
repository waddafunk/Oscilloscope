/*
  ==============================================================================

    ScopeDataCollector.h
    Created: 24 Feb 2023 6:49:28pm
    Author:  wadda

  ==============================================================================
*/

#pragma once
#include "AudioBufferQueue.h"

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
    {}

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

        if (state == State::waitingForTrigger)
        {
            while (index++ < numSamples)
            {
                auto currentSample = *data++;

                if (currentSample >= triggerLevel && prevSample < triggerLevel)
                {
                    numCollected = 0;
                    state = State::collecting;
                    break;
                }

                prevSample = currentSample;
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
                    state = State::waitingForTrigger;
                    prevSample = SampleType(100);
                    break;
                }
            }
        }
    }

private:
    //==============================================================================
    AudioBufferQueue<SampleType>& audioBufferQueue; /**< AudioBufferQueue */
    std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer; /**< Buffer to fill and push to audioBufferQueue */
    size_t numCollected; /**< Number of samples collected. */
    SampleType prevSample = SampleType(100); /**< Last sample collected. */

    static constexpr auto triggerLevel = SampleType(0.05); /**< Level above which the oscilloscope starts drawing the waveform. */
    /**
     * States of the class.
     */
    enum class State { waitingForTrigger, collecting } state{ State::waitingForTrigger };
};