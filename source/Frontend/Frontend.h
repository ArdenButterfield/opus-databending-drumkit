//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_FRONTEND_H
#define OPUSDATABENDINGDRUMKIT_FRONTEND_H

#include "juce_gui_basics/juce_gui_basics.h"

#include "NoteGrid.h"
#include "../State/SynthState.h"

class Frontend : public juce::Component, public juce::Timer
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
};

#endif //OPUSDATABENDINGDRUMKIT_FRONTEND_H
