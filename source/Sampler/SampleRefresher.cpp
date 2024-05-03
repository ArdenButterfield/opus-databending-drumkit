//
// Created by arden on 4/24/24.
//

#include "SampleRefresher.h"

SampleRefresher::SampleRefresher(SampleBuilder& b, SynthState& s) : sampleBuilder(b), synthState(s)
{
    startTimerHz(30);
}

SampleRefresher::~SampleRefresher()
{

}

void SampleRefresher::timerCallback()
{
    for (int m = 0; m < 128; ++m) {
        auto state = synthState.getNoteState(m);
        if (state != nullptr) {
            sampleBuilder.loadSample(*state);
        }
    }
}