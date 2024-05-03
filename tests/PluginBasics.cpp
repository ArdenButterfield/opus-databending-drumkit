#include "helpers/test_helpers.h"
#include <PluginProcessor.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <SampleComparer.h>

#include <cmath>

TEST_CASE ("one is equal to one", "[dummy]")
{
    REQUIRE (1 == 1);
}

TEST_CASE ("Plugin instance", "[instance]")
{
    PluginProcessor testPlugin;

    // This lets us use JUCE's MessageManager without leaking.
    // PluginProcessor might need this if you use the APVTS for example.
    // You'll also need it for tests that rely on juce::Graphics, juce::Timer, etc.
    auto gui = juce::ScopedJuceInitialiser_GUI {};

    SECTION ("name")
    {
        CHECK_THAT (testPlugin.getName().toStdString(),
            Catch::Matchers::Equals ("Opus Databending Drumkit"));
    }
}

TEST_CASE("Prepare to play", "[prepare]")
{
    PluginProcessor testPlugin;

    // This lets us use JUCE's MessageManager without leaking.
    // PluginProcessor might need this if you use the APVTS for example.
    // You'll also need it for tests that rely on juce::Graphics, juce::Timer, etc.
    auto gui = juce::ScopedJuceInitialiser_GUI {};

    testPlugin.prepareToPlay(44100, 512);
}

void fillWithSine(juce::AudioBuffer<float>& buf, float freq, float samplerate)
{
    for (int i = 0; i < buf.getNumSamples(); ++i) {
        buf.setSample (0, i, std::sin (freq * i * juce::MathConstants<float>::twoPi / samplerate));
    }

}

TEST_CASE("Compare", "[compare]")
{
    int length = 1140;
    float samplerate = 48000;
    auto comparer = SampleComparer(length, samplerate);

    auto a = juce::AudioBuffer<float>(1, length);
    auto b = juce::AudioBuffer<float>(1, length);
    a.clear();
    b.clear();
    auto score = comparer.similarityScore(a, b);
    REQUIRE(juce::approximatelyEqual(score, 1.f));

    auto hz300 = juce::AudioBuffer<float>(1,length);
    fillWithSine(hz300, 300, samplerate);
    auto hz305 = juce::AudioBuffer<float>(1,length);
    fillWithSine(hz305, 310, samplerate);
    auto hz2000 = juce::AudioBuffer<float>(1,length);
    fillWithSine(hz2000, 2000, samplerate);
    std::cout << "comparing 300 hz and 305 hz\n";
    auto similarity1 = comparer.similarityScore(hz300, hz305);
    std::cout << "comparing 300 hz and 2000 hz\n";
    auto similarity2 = comparer.similarityScore(hz300, hz2000);
    REQUIRE(similarity1 > similarity2);


    auto random = juce::Random();
    random.setSeedRandomly();
    for (int testNum = 0; testNum < 20; ++testNum) {
        auto buffers = std::vector<juce::AudioBuffer<float>>(3, {1, length});
        auto freqs = std::vector<int>(3);
        freqs[0] = random.nextInt({20, 2000});
        freqs[1] = random.nextInt({(int)(freqs[0]), (int)(freqs[0] * 1.5)});
        freqs[2] = random.nextInt({(int)(freqs[0] * 2), 18000});
        for (unsigned i = 0; i < 3; ++i) {
            fillWithSine(buffers[i], freqs[i], samplerate);
        }
        similarity1 = comparer.similarityScore(buffers[0], buffers[1]);
        similarity2 = comparer.similarityScore(buffers[0], buffers[2]);
        REQUIRE(similarity1 > similarity2);
    }
}

#ifdef PAMPLEJUCE_IPP
    #include <ipp.h>

TEST_CASE ("IPP version", "[ipp]")
{
    CHECK_THAT (ippsGetLibVersion()->Version, Catch::Matchers::Equals ("2021.10.1 (r0x8e799c51)"));
}
#endif
