//
// Created by arden on 4/24/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H
#define OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H

#include "State/SynthState.h"

#include "juce_audio_basics/juce_audio_basics.h"

namespace std
{
template <>
struct hash<NoteState>
{
    std::size_t operator()(const NoteState& noteState) const
    {
        return hash<std::string>()(noteState.toString());
    }
};
}

class SampleBuilder
{
public:
    SampleBuilder();
    ~SampleBuilder();

    juce::AudioBuffer<float>* getSample(NoteState& note);
private:
    std::unordered_map<NoteState, juce::AudioBuffer<float>> memoizedSamples;
};

#endif //OPUSDATABENDINGDRUMKIT_SAMPLEBUILDER_H
