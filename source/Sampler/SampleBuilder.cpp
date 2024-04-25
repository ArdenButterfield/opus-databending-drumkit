//
// Created by arden on 4/24/24.
//

#include "SampleBuilder.h"
#include "SampleLoader.h"

#include "opus.h"

SampleBuilder::SampleBuilder()
{

}

SampleBuilder::~SampleBuilder()
{

}

juce::AudioBuffer<float>* SampleBuilder::getSampleNoBlocking (NoteState& note)
{
    auto sample = memoizedSamples.find(note);

    if (sample != memoizedSamples.end()) {
        return &(sample->second);
    }
    return nullptr;
}

void SampleBuilder::loadSample(NoteState& note)
{
    auto overshoot = NoteState(note.seed, note.noteGenIndex + 5);
    auto sample = memoizedSamples.find(overshoot);

    if (sample != memoizedSamples.end()) {
        return;
    }

    threadPool.addJob(new SampleLoader(memoizedSamples, overshoot), true);
}