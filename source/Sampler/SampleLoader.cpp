//
// Created by arden on 4/24/24.
//

#include "SampleLoader.h"
#include "opus.h"

SampleLoader::SampleLoader(std::unordered_map<NoteState, juce::AudioBuffer<float>>& m, NoteState n)
    : juce::ThreadPoolJob(n.toString()), memoizedSamples(m), noteState(n)
{

}

SampleLoader::~SampleLoader() noexcept
{

}

juce::ThreadPoolJob::JobStatus SampleLoader::runJob()
{
    auto sample = memoizedSamples.find(noteState);

    if (sample != memoizedSamples.end()) {
        return jobHasFinished;
    }

    auto random = juce::Random();
    random.setSeed(noteState.seed);


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

    for (unsigned gen = 0; gen <= noteState.noteGenIndex; ++gen) {
        random.fillBitsRandomly(&(data[2]), (data.size() - 2));
        auto n = NoteState(noteState.seed, gen);

        if (memoizedSamples.find(n) != memoizedSamples.end()) {
            continue;
        }

        int err = OPUS_INTERNAL_ERROR;
        auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
        if (err != OPUS_OK || decoder == nullptr) {
            DBG ( "Creating decoder failed" );
            continue;
        }

        buffer.clear();

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

        auto mag = buffer.getMagnitude(0, buffer.getNumSamples());
        if (mag > 0) {
            buffer.applyGain(1 / mag);
        }

        memoizedSamples[{noteState.seed, gen}] = buffer;
        DBG("built note: " << noteState.seed << " " << juce::String(gen));

        opus_decoder_destroy(decoder);
    }

    return jobHasFinished;
}