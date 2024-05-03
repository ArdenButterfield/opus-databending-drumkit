//
// Created by arden on 4/24/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H
#define OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H

#include "../State/SynthState.h"

#include "juce_audio_basics/juce_audio_basics.h"

#include "juce_events/juce_events.h"

class SampleBuilder
{
public:
    SampleBuilder();
    ~SampleBuilder();

    void loadSample(NoteState& note);
    juce::AudioBuffer<float>* getSampleNoBlocking(NoteState& note);

private:
    juce::ThreadPool threadPool;

    std::unordered_map<NoteState, juce::AudioBuffer<float>> memoizedSamples;
};

#endif //OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H
