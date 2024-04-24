//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_NOTESLOT_H
#define OPUSDATABENDINGDRUMKIT_NOTESLOT_H

#include "juce_gui_basics/juce_gui_basics.h"

#include "NoteInGrid.h"
#include "../State/SynthState.h"

class NoteSlot : public juce::Component
{
public:
    NoteSlot(SynthState& state, int midiNote);
    ~NoteSlot();
    void paint(juce::Graphics &g) override;
    void resized() override;

    void mouseUp(const juce::MouseEvent &event) override;
    const int midiNote;
private:
    SynthState& synthState;
    std::unique_ptr<NoteInGrid> note;
};

#endif //OPUSDATABENDINGDRUMKIT_NOTESLOT_H
