//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_NOTEGRID_H
#define OPUSDATABENDINGDRUMKIT_NOTEGRID_H

#include "juce_gui_basics/juce_gui_basics.h"

#include "NoteSlot.h"
#include "../State/SynthState.h"

class NoteGrid : public juce::Component
{
public:
    NoteGrid(SynthState& state, int minNote, int maxNote, int notesPerRow);
    ~NoteGrid();
    void resized() override;
    void paint(juce::Graphics &g) override;

    int desiredHeight(int width);

private:
    SynthState& synthState;
    std::vector<std::unique_ptr<NoteSlot>> noteSlots;
    const int minNote;
    const int maxNote;
    const int notesPerRow;
};

#endif //OPUSDATABENDINGDRUMKIT_NOTEGRID_H
