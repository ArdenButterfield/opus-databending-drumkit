//
// Created by arden on 4/22/24.
//

#include "OpusSynthVoice.h"
#include "OpusSynthSound.h"

OpusSynthVoice::OpusSynthVoice(SynthState& s, SampleBuilder& b) : synthState(s), sampleBuilder(b), buffer(nullptr), bufferPosition(0)
{

}

bool OpusSynthVoice::canPlaySound (juce::SynthesiserSound* synthesiserSound)
{
    if (dynamic_cast<OpusSynthSound*>(synthesiserSound) != nullptr) {
        return true;
    }
    return false;
}

void OpusSynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* synthesiserSound, int)
{
    synthState.setNoteBeingPlayed(midiNoteNumber, true);
    DBG( "start note " << midiNoteNumber );
    auto note = synthState.getNoteState(midiNoteNumber);
    if (note != nullptr) {
        buffer = sampleBuilder.getSampleNoBlocking(*note);
        bufferPosition = 0;
    }
}

void OpusSynthVoice::stopNote (float, bool allowTailOff)
{
    synthState.setNoteBeingPlayed(getCurrentlyPlayingNote(), false);
}

void OpusSynthVoice::pitchWheelMoved (int)
{

}

void OpusSynthVoice::controllerMoved (int, int)
{

}

void OpusSynthVoice::renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if (buffer == nullptr || bufferPosition >= buffer->getNumSamples()) {
        return;
    }
    int numSamplesToCopy = juce::jmin(numSamples, buffer->getNumSamples() - bufferPosition);
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom (channel, startSample, buffer->getReadPointer (0, bufferPosition), numSamplesToCopy);
    }
    bufferPosition += numSamplesToCopy;
}

void OpusSynthVoice::setCurrentPlaybackSampleRate (double newRate)
{

}

