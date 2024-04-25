//
// Created by arden on 4/24/24.
//

#include "ChangingSamplesMonitor.h"

ChangingSamplesMonitor::ChangingSamplesMonitor (SampleBuilder& b, SynthState& s) : synthState(s), sampleBuilder(b)
{

}

ChangingSamplesMonitor::~ChangingSamplesMonitor()
{

}

void ChangingSamplesMonitor::processBlock (juce::AudioBuffer<float>& buffer)
{
    if (!synthState.monitorSwitchingSamples) {
        synthState.midiPitchToMonitor = -1;
        return;
    }

    int pitch = synthState.midiPitchToMonitor;
    if (pitch >= 0) {
        synthState.midiPitchToMonitor = -1;
        auto note = synthState.getNoteState(pitch);
        if (note != nullptr) {
            activeSample = sampleBuilder.getSampleNoBlocking(*note);
            position = 0;
        }
    }
    if (activeSample == nullptr || position >= activeSample->getNumSamples()) {
        return;
    }

    int numSamplesToCopy = juce::jmin(buffer.getNumSamples(), activeSample->getNumSamples() - position);
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        buffer.addFrom (channel, 0, activeSample->getReadPointer (0, position), numSamplesToCopy);
    }
    position += numSamplesToCopy;

}