//
// Created by arden on 4/24/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SAMPLELOADER_H
#define OPUSDATABENDINGDRUMKIT_SAMPLELOADER_H

#include "../State/SynthState.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_gui_basics/juce_gui_basics.h"

class SampleLoader : public juce::ThreadPoolJob
{
public:
    SampleLoader(std::unordered_map<NoteState, juce::AudioBuffer<float>>& memoizedSamples, NoteState noteState);
    ~SampleLoader() override;
    JobStatus runJob() override;
private:
    std::unordered_map<NoteState, juce::AudioBuffer<float>>& memoizedSamples;
    NoteState noteState;
};

#endif //OPUSDATABENDINGDRUMKIT_SAMPLELOADER_H
