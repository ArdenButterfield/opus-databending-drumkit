#ifndef SKIP_EXPLORATION

#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "opus.h"

#include "juce_core/juce_core.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_formats/juce_audio_formats.h"

#if 0
TEST_CASE ("bitflipstep", "[bitflipstep]")
{
    auto random = juce::Random();
    random.setSeed(12345);

    int err = OPUS_INTERNAL_ERROR;
    auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
    if (err != OPUS_OK || decoder == nullptr) {
        DBG ( "Creating decoder failed" );
    }

    const auto frameSize = 1440;
    const auto numFrames = 4;
    auto buffer = juce::AudioBuffer<float>();
    buffer.setSize(1, frameSize * numFrames);
    buffer.clear();
    auto data = std::vector<unsigned char>(185);
    data[0] = 115;
    data[1] = 3;

    auto headers = std::vector<unsigned char>(2);
    headers[0] = data[0];
    headers[1] = data[1];

    for (int i = 0; i <= numFrames; ++i) {
        random.fillBitsRandomly(&(data[2]), (data.size() - 2));
    }

    auto outputFile = juce::File("/tmp/basic_opus_hit.wav");
    auto outStream = outputFile.createOutputStream();
    auto format = juce::WavAudioFormat();
    auto arr = juce::StringPairArray();
    auto writer = format.createWriterFor(outStream.get(), 44100.0, buffer.getNumChannels(), 32, arr, 0);

    for (int step = 0; step < 2000; step++) {
        std::cout << step << "\n";
        auto numSamplesDecoded = 0;
        for (int i = 0; i < numFrames; ++i) {
            auto decodedInRound = opus_decode_float(
                decoder,
                (i == 0) ? &data[0] : &headers[0],
                (i == 0) ? data.size() : headers.size(),
                buffer.getWritePointer(0, numSamplesDecoded),
                frameSize,
                0);
            numSamplesDecoded += decodedInRound;
        }

        buffer.applyGain(1 / buffer.getMagnitude(0, buffer.getNumSamples()));

        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());

        data[random.nextInt({2,static_cast<int>(data.size())})] ^= (1 << random.nextInt(8));
    }
}

TEST_CASE ("bitflip_tryall", "[bitflip_tryall]")
{
    auto random = juce::Random();
    random.setSeed(89898);

    const auto frameSize = 1440;
    const auto numFrames = 4;
    auto buffer = juce::AudioBuffer<float>();
    buffer.setSize(1, frameSize * numFrames);
    buffer.clear();
    auto data = std::vector<unsigned char>(185);
    data[0] = 115;
    data[1] = 3;

    auto headers = std::vector<unsigned char>(2);
    headers[0] = data[0];
    headers[1] = data[1];

    for (int i = 0; i <= numFrames; ++i) {
        random.fillBitsRandomly(&(data[2]), (data.size() - 2));
    }

    auto outputFile = juce::File("/tmp/bitflip_all.wav");
    auto outStream = outputFile.createOutputStream();
    auto format = juce::WavAudioFormat();
    auto arr = juce::StringPairArray();
    auto writer = format.createWriterFor(outStream.get(), 44100.0, buffer.getNumChannels(), 32, arr, 0);
    for (int step = 0; step < (data.size() - 2) * 8; step++) {

        int err = OPUS_INTERNAL_ERROR;
        auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
        if (err != OPUS_OK || decoder == nullptr) {
            DBG ( "Creating decoder failed" );
        }


        std::cout << step << "\n";
        auto modData = data;
        modData[2 + step / 8] ^= (1 << (step % 8));
        auto numSamplesDecoded = 0;
        for (int i = 0; i < numFrames; ++i) {
            auto decodedInRound = opus_decode_float(
                decoder,
                (i == 0) ? &modData[0] : &headers[0],
                (i == 0) ? modData.size() : headers.size(),
                buffer.getWritePointer(0, numSamplesDecoded),
                frameSize,
                0);
            numSamplesDecoded += decodedInRound;
        }

        opus_decoder_destroy(decoder);

        buffer.applyGain(1 / buffer.getMagnitude(0, buffer.getNumSamples()));

        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
}


#endif

#include "SampleComparer.h"
const auto frameSize = 1440;

const auto numFrames = 1;

void fillBuffer(std::vector<unsigned char>& data, juce::AudioBuffer<float>& buffer)
{
    int err = OPUS_INTERNAL_ERROR;
    auto decoder = opus_decoder_create (static_cast<int> (48000), 1, &err);
    if (err != OPUS_OK || decoder == nullptr)
    {
        DBG ("Creating decoder failed");
    }

    auto numSamplesDecoded = 0;
    for (int i = 0; i < numFrames; ++i)
    {
        auto decodedInRound = opus_decode_float (
            decoder,
            &data[0],
            (i == 0) ? data.size() : 2,
            buffer.getWritePointer (0, numSamplesDecoded),
            frameSize,
            0);
        numSamplesDecoded += decodedInRound;
    }

    opus_decoder_destroy (decoder);

    auto rescale_factor = 0.5f / buffer.getMagnitude (0, buffer.getNumSamples());
    buffer.applyGain (rescale_factor);

}

void morphToDestination() {
    auto comparer = SampleComparer(frameSize * numFrames, 48000);

    auto random = juce::Random();
    auto start_seed = random.nextInt();
    auto end_seed = random.nextInt();

    auto buffer = juce::AudioBuffer<float>();
    buffer.setSize (1, frameSize * numFrames);
    buffer.clear();
    auto start_buffer = juce::AudioBuffer<float>();
    start_buffer.setSize (1, frameSize * numFrames);
    start_buffer.clear();
    auto end_buffer = juce::AudioBuffer<float>();
    end_buffer.setSize (1, frameSize * numFrames);
    end_buffer.clear();



    auto outputFile = juce::File ("/home/arden/projects/opus_drumkit/morph_with_direction/morph_"
                                  + juce::String (start_seed) + "_" + juce::String (end_seed) + ".wav");
    auto outStream = outputFile.createOutputStream();
    auto format = juce::WavAudioFormat();
    auto arr = juce::StringPairArray();
    auto writer = format.createWriterFor (outStream.get(), 48000, 1, 32, arr, 0);


    auto start_data = std::vector<unsigned char> (185);
    start_data[0] = 115;
    start_data[1] = 3;

    auto end_data = std::vector<unsigned char> (185);
    end_data[0] = 115;
    end_data[1] = 3;

    random.setSeed(start_seed);
    random.fillBitsRandomly (&(start_data[2]), (start_data.size() - 2));
    random.setSeed(end_seed);
    random.fillBitsRandomly (&(end_data[2]), (end_data.size() - 2));

    fillBuffer(start_data, start_buffer);
    fillBuffer(end_data, end_buffer);

    writer->writeFromAudioSampleBuffer(start_buffer, 0, start_buffer.getNumSamples());
    writer->writeFromAudioSampleBuffer(end_buffer, 0, end_buffer.getNumSamples());


    auto angle = std::acos(comparer.similarityScore(start_buffer, end_buffer));

    std::unordered_set<unsigned int> bitsToFlip;
    for (auto byte = 2; byte < start_data.size(); ++byte) {
        for (auto bit = 0; bit < 8; ++bit) {
            if ((start_data[byte] & (1 << bit)) != (end_data[byte] & (1 << bit))) {
                bitsToFlip.insert(byte * 8 + bit);
            }
        }
    }

    auto data = start_data;

    auto steps = static_cast<float>(bitsToFlip.size());

    while (!bitsToFlip.empty()) {
        auto best_flip = *bitsToFlip.begin();
        auto offTarget = 10.f;

        auto amountCovered = (steps - bitsToFlip.size()) / steps;
        auto targetAngleFromStart = angle * amountCovered;
        auto targetAngleFromEnd = angle * (1 - amountCovered);

        auto targetSimilarityToStart = std::cos(targetAngleFromStart);
        auto targetSimilarityToEnd = std::cos(targetAngleFromEnd);

        for (auto i : bitsToFlip) {
            // make flip
            data[i / 8] ^= (1 << (i % 8));
            fillBuffer(data, buffer);

            auto similarityToStart = comparer.similarityScore(start_buffer, buffer);
            auto similarityToEnd = comparer.similarityScore(end_buffer, buffer);

            auto difference = (similarityToStart - targetSimilarityToStart) * (similarityToStart - targetSimilarityToStart) +
                              (similarityToEnd - targetSimilarityToEnd) * (similarityToEnd - targetSimilarityToEnd);

            if (difference < offTarget) {
                offTarget = difference;
                best_flip = i;
            }

            // undo flip
            data[i / 8] ^= (1 << (i % 8));
        }

        data[best_flip / 8] ^= (1 << (best_flip % 8));
        fillBuffer(data, buffer);
        bitsToFlip.erase(best_flip);
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
    }
}

TEST_CASE("morph to destination", "[morphtodest]")
{
    juce::ThreadPool pool;
    for (int iteration = 0; iteration < 6; ++iteration) {
        pool.addJob(morphToDestination);
    }
    while (pool.getNumJobs() > 0) {
        pool.waitForJobToFinish(pool.getJob(0), 100000000);
    }
}

#if 0
TEST_CASE("morph over time", "[morph]")
{
    const auto frameSize = 1440;
    const auto numFrames = 4;

    auto comparer = SampleComparer(frameSize * numFrames, 48000);

    for (int seed = 1225; seed < 1235; ++seed) {
        auto random = juce::Random();
        random.setSeed (seed);

        auto initialBuffer = juce::AudioBuffer<float>();
        initialBuffer.setSize (1, frameSize * numFrames);
        initialBuffer.clear();
        auto data = std::vector<unsigned char> (185);
        data[0] = 115;
        data[1] = 3;

        auto headers = std::vector<unsigned char> (2);
        headers[0] = data[0];
        headers[1] = data[1];

        for (int i = 0; i <= numFrames; ++i)
        {
            random.fillBitsRandomly (&(data[2]), (data.size() - 2));
        }

        auto outputFile = juce::File ("/home/arden/projects/opus_drumkit/morph/morph_" + juce::String (seed) + ".wav");
        auto outStream = outputFile.createOutputStream();
        auto format = juce::WavAudioFormat();
        auto arr = juce::StringPairArray();
        auto writer = format.createWriterFor (outStream.get(), 44100.0, initialBuffer.getNumChannels(), 32, arr, 0);

        int err = OPUS_INTERNAL_ERROR;
        auto decoder = opus_decoder_create (static_cast<int> (48000), 1, &err);
        if (err != OPUS_OK || decoder == nullptr)
        {
            DBG ("Creating decoder failed");
        }

        auto numSamplesDecoded = 0;
        for (int i = 0; i < numFrames; ++i)
        {
            auto decodedInRound = opus_decode_float (
                decoder,
                (i == 0) ? &data[0] : &headers[0],
                (i == 0) ? data.size() : headers.size(),
                initialBuffer.getWritePointer (0, numSamplesDecoded),
                frameSize,
                0);
            numSamplesDecoded += decodedInRound;
        }

        opus_decoder_destroy (decoder);

        auto rescale_factor = 0.5f / initialBuffer.getMagnitude (0, initialBuffer.getNumSamples());
        initialBuffer.applyGain (rescale_factor);

        writer->writeFromAudioSampleBuffer (initialBuffer, 0, initialBuffer.getNumSamples());

        juce::AudioBuffer<float> currentBuffer;
        juce::AudioBuffer<float> nextBuffer;
        juce::AudioBuffer<float> previousBuffer;
        juce::AudioBuffer<float> testBuffer;
        currentBuffer.setSize (1, frameSize * numFrames);
        nextBuffer.setSize (1, frameSize * numFrames);
        previousBuffer.setSize (1, frameSize * numFrames);
        testBuffer.setSize (1, frameSize * numFrames);

        previousBuffer.clear();
        nextBuffer.clear();
        currentBuffer = initialBuffer;

        std::cout << "morphing\n";
        for (int morphStep = 0; morphStep < 100; ++morphStep) {

            auto bestDifference = 10.f;
            auto best_i = -1;
            for (int step = 0; step < (data.size() - 2) * 8; step++)
            {
                if (random.nextFloat() > 0.1) {
                    continue;
                }
                int err = OPUS_INTERNAL_ERROR;
                auto decoder = opus_decoder_create (static_cast<int> (48000), 1, &err);
                if (err != OPUS_OK || decoder == nullptr)
                {
                    DBG ("Creating decoder failed");
                }
                auto modData = data;
                modData[2 + step / 8] ^= (1 << (step % 8));

                auto numSamplesDecoded = 0;
                for (int i = 0; i < numFrames; ++i)
                {
                    auto decodedInRound = opus_decode_float (
                        decoder,
                        (i == 0) ? &modData[0] : &headers[0],
                        (i == 0) ? modData.size() : headers.size(),
                        testBuffer.getWritePointer (0, numSamplesDecoded),
                        frameSize,
                        0);
                    numSamplesDecoded += decodedInRound;
                }

                opus_decoder_destroy (decoder);
                auto rescale_factor = 0.5f / testBuffer.getMagnitude (0, currentBuffer.getNumSamples());
                testBuffer.applyGain (rescale_factor);
                auto targetSimilarity = 0.0f + (random.nextFloat() * 0.01);
                auto difference = std::abs(comparer.similarityScore(testBuffer, currentBuffer) - targetSimilarity);
                if ((difference < bestDifference)/* && (comparer.similarityScore(testBuffer, initialBuffer) < comparer.similarityScore(previousBuffer, initialBuffer))*/) {
                    bestDifference = difference;
                    nextBuffer = testBuffer;
                    best_i = step;
                }
            }
            data[2 + best_i / 8] ^= (1 << (best_i % 8));

            std::cout << bestDifference << " " << best_i << "\n";
            previousBuffer = currentBuffer;
            currentBuffer = nextBuffer;
            writer->writeFromAudioSampleBuffer(nextBuffer, 0, nextBuffer.getNumSamples());
        }
    }
}
#endif

#if false

TEST_CASE ("bitflip_tryall_gendata", "[bitflip_tryallgendata]")
{
    for (int seed = 0; seed < 500; ++seed) {
        std::cout <<seed << "\n";
        auto random = juce::Random();
        random.setSeed(seed);

        const auto frameSize = 1440;
        const auto numFrames = 1;
        auto buffer = juce::AudioBuffer<float>();
        buffer.setSize(1, frameSize * numFrames);
        buffer.clear();
        auto data = std::vector<unsigned char>(185);
        data[0] = 115;
        data[1] = 3;

        auto headers = std::vector<unsigned char>(2);
        headers[0] = data[0];
        headers[1] = data[1];

        for (int i = 0; i <= numFrames; ++i) {
            random.fillBitsRandomly(&(data[2]), (data.size() - 2));
        }

        auto outputFile = juce::File("/home/arden/projects/opus_drumkit/samples_wavetable/bitflip_" + juce::String(seed) + ".wav");
        auto outStream = outputFile.createOutputStream();
        auto format = juce::WavAudioFormat();
        auto arr = juce::StringPairArray();
        auto writer = format.createWriterFor(outStream.get(), 44100.0, buffer.getNumChannels(), 32, arr, 0);

        int err = OPUS_INTERNAL_ERROR;
        auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
        if (err != OPUS_OK || decoder == nullptr) {
            DBG ( "Creating decoder failed" );
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
            numSamplesDecoded += decodedInRound;
        }

        opus_decoder_destroy(decoder);

        auto rescale_factor = 0.5f / buffer.getMagnitude(0, buffer.getNumSamples());

        buffer.applyGain(rescale_factor);

        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        buffer.clear();
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());



        for (int step = 0; step < (data.size() - 2) * 8; step++) {

            int err = OPUS_INTERNAL_ERROR;
            auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
            if (err != OPUS_OK || decoder == nullptr) {
                DBG ( "Creating decoder failed" );
            }
            auto modData = data;
            modData[2 + step / 8] ^= (1 << (step % 8));

            auto numSamplesDecoded = 0;
            for (int i = 0; i < numFrames; ++i) {
                auto decodedInRound = opus_decode_float(
                    decoder,
                    (i == 0) ? &modData[0] : &headers[0],
                    (i == 0) ? modData.size() : headers.size(),
                    buffer.getWritePointer(0, numSamplesDecoded),
                    frameSize,
                    0);
                numSamplesDecoded += decodedInRound;
            }

            opus_decoder_destroy(decoder);
            auto rescale_factor = 0.5f / buffer.getMagnitude(0, buffer.getNumSamples());
            buffer.applyGain(rescale_factor);

            writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }
    }
}
#endif

#endif // SKIP_EXPLORATION