//
// Created by arden on 4/23/24.
//

#include "NoteGrid.h"

NoteGrid::NoteGrid(SynthState& state, const int _minNote, const  int _maxNote, const int _notesPerRow)
    : synthState(state), minNote(_minNote), maxNote(_maxNote), notesPerRow(_notesPerRow)
{
    for (auto note = minNote; note <= maxNote; ++note) {
        noteSlots.emplace_back(std::make_unique<NoteSlot>(synthState, note));
    }
}

NoteGrid::~NoteGrid() {

}

void NoteGrid::resized()
{
    auto numRows = noteSlots.size() / notesPerRow;

    auto noteWidth = getWidth() / notesPerRow;
    auto noteHeight = noteWidth;

    for (auto i = 0; i < noteSlots.size(); ++i) {
        auto row = numRows - 1 - i / notesPerRow;
        auto col = i % notesPerRow;
        noteSlots[i]->setBounds(col * noteHeight, row * noteWidth, noteWidth, noteHeight);
    }
    for (auto& noteSlot : noteSlots) {
        addAndMakeVisible(noteSlot.get());
    }
}

void NoteGrid::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::green);
}
int NoteGrid::desiredHeight (int width)
{
    int numRows = noteSlots.size() / notesPerRow;
    if (noteSlots.size() % notesPerRow) {
        ++numRows;
    }

    return width * numRows / notesPerRow;
}
