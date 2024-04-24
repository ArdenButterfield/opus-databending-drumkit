//
// Created by arden on 4/24/24.
//

#include "SampleBuilder.h"
#include <opus.h>

SampleBuilder::SampleBuilder()
{
}

SampleBuilder::~SampleBuilder()
{

}

juce::AudioBuffer<float>* SampleBuilder::getSample(NoteState& note)
{
    auto sample = memoizedSamples.find(note);

    if (sample != memoizedSamples.end()) {
        DBG( "sample found for " << note.toString());
        return &(sample->second);
    }

    auto random = juce::Random();
    random.setSeed(note.seed);

    int err = OPUS_INTERNAL_ERROR;
    auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
    if (err != OPUS_OK || decoder == nullptr) {
        DBG ( "Creating decoder failed" );
        return nullptr;
    }

    const auto frameSize = 1440;
    const auto numFrames = 32;

    juce::AudioBuffer<float> buffer;
    buffer.setSize(1, frameSize * numFrames);
    buffer.clear();

    auto data = std::vector<unsigned char>(185);
    data[0] = 115;
    data[1] = 3;

    auto headers = std::vector<unsigned char>(2);
    headers[0] = data[0];
    headers[1] = data[1];

    for (unsigned gen = 0; gen <= note.noteGenIndex; ++gen) {
        random.fillBitsRandomly(&(data[2]), (data.size() - 2));
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
        memoizedSamples[{note.seed, gen}] = buffer;
        DBG("built note: " << note.toString());
    }

    return &memoizedSamples[note];
}