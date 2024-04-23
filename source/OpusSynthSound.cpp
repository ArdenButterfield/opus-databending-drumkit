//
// Created by arden on 4/22/24.
//

#include "OpusSynthSound.h"
#include <opus.h>

OpusSynthSound::OpusSynthSound (int64_t randomSeed, int n, int note)
{
    auto random = juce::Random();
    random.setSeed(randomSeed);

    int err = OPUS_INTERNAL_ERROR;
    auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
    if (err != OPUS_OK || decoder == nullptr) {
        DBG ( "Creating decoder failed" );
    }

    const auto frameSize = 1440;
    const auto numFrames = 32;
    buffer.setSize(1, frameSize * numFrames);

    auto data = std::vector<unsigned char>(185);
    data[0] = 115;
    data[1] = 3;

    auto headers = std::vector<unsigned char>(2);
    headers[0] = data[0];
    headers[1] = data[1];

    for (int i = 0; i <= n; ++i) {
        random.fillBitsRandomly(&(data[2]), (data.size() - 2));
    }

    auto numSamplesDecoded = 0;
    for (int i = 0; i < numFrames; ++i) {
        auto decodedInRound = opus_decode_float(
            decoder,
            (i == 0) ? &data[0] : &headers[0],
            (i == 0) ? data.size() : headers.size(),
            buffer.getWritePointer(0, numSamplesDecoded),
            frameSize,
            0);
        DBG( decodedInRound << " samples decoded");
        numSamplesDecoded += decodedInRound;
    }
}
const juce::AudioBuffer<float>* OpusSynthSound::getBuffer()
{
    return &buffer;
}
