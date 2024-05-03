//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_FRONTEND_H
#define OPUSDATABENDINGDRUMKIT_FRONTEND_H

#include "juce_gui_basics/juce_gui_basics.h"

#include "NoteGrid.h"
#include "../State/SynthState.h"

class Frontend : public juce::Component, public juce::Button::Listener, public juce::Timer
{
public:
    Frontend(SynthState& state);
    ~Frontend() override;
    void paint(juce::Graphics &g) override;
    void resized() override;
private:
    SynthState& synthState;
    juce::Viewport viewport;
    NoteGrid noteGrid;

    juce::ToggleButton monitorChangingSamplesButton;

    void timerCallback() override;

    void buttonStateChanged(juce::Button *) override;
    void buttonClicked(juce::Button *) override;
};

#endif //OPUSDATABENDINGDRUMKIT_FRONTEND_H
