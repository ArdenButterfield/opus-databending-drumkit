//
// Created by arden on 4/29/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_CREATESAMPLE_H
#define OPUSDATABENDINGDRUMKIT_CREATESAMPLE_H

#endif //OPUSDATABENDINGDRUMKIT_CREATESAMPLE_H

#include "opus.h"
#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"

void createSample(juce::Random& random, juce::AudioBuffer<float>& buffer)
{
    const auto frameSize = 1440;
    const auto numFrames = 32;

    buffer.setSize(1, frameSize * numFrames);
    buffer.clear();

    auto data = std::vector<unsigned char>(185);
    data[0] = 115;
    data[1] = 3;

    auto headers = std::vector<unsigned char>(2);
    headers[0] = data[0];
    headers[1] = data[1];

    random.fillBitsRandomly(&(data[2]), (data.size() - 2));

    int err = OPUS_INTERNAL_ERROR;
    auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
    if (err != OPUS_OK || decoder == nullptr) {
        DBG ("Creating decoder failed");
        return;
    }

    auto numSamplesDecoded = 0;
    for (unsigned i = 0; i < numFrames; ++i) {
        auto decodedInRound = opus_decode_float(
            decoder,
            (i == 0) ? &data[0] : &headers[0],
            (i == 0) ? data.size() : headers.size(),
            buffer.getWritePointer(0, numSamplesDecoded),
            frameSize,
            0);
        numSamplesDecoded += decodedInRound;
    }

    opus_decoder_destroy(decoder);

    auto mag = buffer.getMagnitude(0, buffer.getNumSamples());
    if (mag > 0) {
        buffer.applyGain(1 / mag);
    }
}