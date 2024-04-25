//
// Created by arden on 4/22/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_OPUSSYNTHVOICE_H
#define OPUSDATABENDINGDRUMKIT_OPUSSYNTHVOICE_H

#include <juce_audio_basics/juce_audio_basics.h>

#include "Sampler/SampleBuilder.h"
#include "State/SynthState.h"

class OpusSynthVoice : public juce::SynthesiserVoice
{
public:
    OpusSynthVoice(SynthState& s, SampleBuilder& b);
    bool canPlaySound (juce::SynthesiserSound* synthesiserSound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* synthesiserSound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int) override;
    void controllerMoved (int, int) override;
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;
    void setCurrentPlaybackSampleRate(double newRate) override;
private:
    SynthState& synthState;
    SampleBuilder& sampleBuilder;
    const juce::AudioBuffer<float>* buffer;

    int bufferPosition;
};

#endif //OPUSDATABENDINGDRUMKIT_OPUSSYNTHVOICE_H
