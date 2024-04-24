#include "PluginEditor.h"
#include "catch2/benchmark/catch_benchmark_all.hpp"
#include "catch2/catch_test_macros.hpp"

TEST_CASE ("Boot performance")
{
//    BENCHMARK_ADVANCED("do nothing")
//    (Catch::Benchmark::Chronometer meter)
//    {
//        int j = 0;
//        meter.measure ([&] (int i) {j += i;});
//        std::cout << j;
//    };

    BENCHMARK_ADVANCED("opus decoder")
    (Catch::Benchmark::Chronometer meter)
    {
        int err = OPUS_INTERNAL_ERROR;
        auto decoder = opus_decoder_create(static_cast<int>(48000), 1, &err);
        if (err != OPUS_OK || decoder == nullptr) {
            DBG ( "Creating decoder failed" );
        }
        juce::AudioBuffer<float> buffer;

        const auto frameSize = 1440;
        const auto numFrames = 32;
        buffer.setSize(1, frameSize * numFrames);
        buffer.clear();
        auto data = std::vector<unsigned char>(185);
        data[0] = 115;
        data[1] = 3;
        meter.measure ([&] (int /* i */) {
            auto decodedInRound = opus_decode_float (
                decoder,
                &data[0],
                data.size(),
                buffer.getWritePointer (0, 0),
                frameSize,
                0);
        });
    };

    BENCHMARK_ADVANCED ("Processor constructor")
    (Catch::Benchmark::Chronometer meter)
    {
        auto gui = juce::ScopedJuceInitialiser_GUI {};
        std::vector<Catch::Benchmark::storage_for<PluginProcessor>> storage (size_t (meter.runs()));
        meter.measure ([&] (int i) { storage[(size_t) i].construct();});
    };

    BENCHMARK_ADVANCED ("Processor destructor")
    (Catch::Benchmark::Chronometer meter)
    {
        auto gui = juce::ScopedJuceInitialiser_GUI {};
        std::vector<Catch::Benchmark::destructable_object<PluginProcessor>> storage (size_t (meter.runs()));
        for (auto& s : storage)
            s.construct();
        meter.measure ([&] (int i) { storage[(size_t) i].destruct(); });
    };

    BENCHMARK_ADVANCED ("Editor open and close")
    (Catch::Benchmark::Chronometer meter)
    {
        auto gui = juce::ScopedJuceInitialiser_GUI {};

        PluginProcessor plugin;

        // due to complex construction logic of the editor, let's measure open/close together
        meter.measure ([&] (int /* i */) {
            auto editor = plugin.createEditorIfNeeded();
            plugin.editorBeingDeleted (editor);
            delete editor;
            return plugin.getActiveEditor();
        });
    };
}
