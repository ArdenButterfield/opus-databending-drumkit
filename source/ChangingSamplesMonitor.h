//
// Created by arden on 4/24/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_CHANGINGSAMPLESMONITOR_H
#define OPUSDATABENDINGDRUMKIT_CHANGINGSAMPLESMONITOR_H

#include "Sampler/SampleBuilder.h"
#include "State/SynthState.h"

#include "juce_events/juce_events.h"

class ChangingSamplesMonitor
{
public:
    ChangingSamplesMonitor(SampleBuilder& b, SynthState& s);
    ~ChangingSamplesMonitor();

    void processBlock(juce::AudioBuffer<float>& buffer);
private:
    SynthState& synthState;
    SampleBuilder& sampleBuilder;
    juce::AudioBuffer<float>* activeSample;
    int position;
};

#endif //OPUSDATABENDINGDRUMKIT_CHANGINGSAMPLESMONITOR_H
