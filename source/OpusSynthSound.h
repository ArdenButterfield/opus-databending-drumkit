//
// Created by arden on 4/22/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H
#define OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H

#include <juce_audio_basics/juce_audio_basics.h>

class OpusSynthSound : public juce::SynthesiserSound
{
public:
    OpusSynthSound(int64_t randomSeed, int n, int note);

    const juce::AudioBuffer<float>* getBuffer();

    bool appliesToNote    (int _note) override { return note == _note; }
    bool appliesToChannel (int) override        { return true; }
private:
    int note;
    juce::AudioBuffer<float> buffer;
};

#endif //OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H
