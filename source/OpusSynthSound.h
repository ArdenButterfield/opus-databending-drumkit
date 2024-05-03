//
// Created by arden on 4/22/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H
#define OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H

#include <juce_audio_basics/juce_audio_basics.h>

class OpusSynthSound : public juce::SynthesiserSound
{
public:
    OpusSynthSound() = default;

    bool appliesToNote    (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
private:
};

#endif //OPUSDATABENDINGDRUMKIT_OPUSSYNTHSOUND_H
