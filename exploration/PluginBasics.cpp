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

float similarity(juce::AudioBuffer<float>& a, juce::AudioBuffer<float>& b)
{
    auto c = juce::AudioBuffer<float>(a.getNumChannels(), juce::jmax(a.getNumSamples(), b.getNumSamples()));
    c.clear();
    c.addFrom(0,0,a,0,0,a.getNumSamples(), 1);
    c.addFrom(0,0,b,0,0,b.getNumSamples(), -1);
    auto similarity = c.getRMSLevel(0,0,c.getNumSamples());
    return similarity;
}

TEST_CASE("morph over time", "[morph]")
{
    for (int seed = 111100; seed < 111130; ++seed) {
        auto random = juce::Random();
        random.setSeed (seed);

        const auto frameSize = 1440;
        const auto numFrames = 4;
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
        for (int morphStep = 0; morphStep < 2000; ++morphStep) {

            auto bestDifference = 10.0f;
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
                auto targetSimilarity = 0.01f;
                auto difference = std::abs(similarity(testBuffer, currentBuffer) - targetSimilarity);
                if ((difference < bestDifference) && (similarity(testBuffer, initialBuffer) > similarity(previousBuffer, initialBuffer))) {
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