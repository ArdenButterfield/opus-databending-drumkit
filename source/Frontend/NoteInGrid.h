//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_NOTEINGRID_H
#define OPUSDATABENDINGDRUMKIT_NOTEINGRID_H

#include "juce_gui_basics/juce_gui_basics.h"
#include "../State/SynthState.h"

class NoteSlot;

class NoteInGrid : public juce::Component, public juce::Button::Listener, public juce::Timer
{
public:
    NoteInGrid(SynthState& state, NoteSlot* slot);
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    void buttonClicked(juce::Button* b) override;
    void buttonStateChanged(juce::Button* b) override;

    void timerCallback() override;
    NoteSlot* homeSlot;
    SynthState& synthState;
    juce::ArrowButton previousSound;
    juce::ArrowButton nextSound;
};

#endif //OPUSDATABENDINGDRUMKIT_NOTEINGRID_H
