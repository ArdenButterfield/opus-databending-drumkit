//
// Created by arden on 4/24/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SAMPLEREFRESHER_H
#define OPUSDATABENDINGDRUMKIT_SAMPLEREFRESHER_H

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"

#include "SampleBuilder.h"
#include "../State/SynthState.h"

class SampleRefresher : juce::Timer
{
public:
    SampleRefresher(SampleBuilder& b, SynthState& s);
    ~SampleRefresher();
    void timerCallback() override;
private:

    SampleBuilder& sampleBuilder;
    SynthState& synthState;
};

#endif //OPUSDATABENDINGDRUMKIT_SAMPLEREFRESHER_H
