//
// Created by arden on 4/23/24.
//

#include "SynthState.h"


SynthState::SynthState (int _minPitch, int _maxPitch, NoteState _defaultNote) : minPitch(_minPitch), maxPitch(_maxPitch), defaultNote(_defaultNote)
{
    noteMaps.resize(maxPitch - minPitch);
    notesBeingPlayed.resize(maxPitch - minPitch);
}

SynthState::~SynthState()
{

}

void SynthState::addNoteDefault (int midiPitch)
{
    addNote(midiPitch, defaultNote);
}



void SynthState::addNote(int midiPitch, const NoteState& noteState)
{
    if (midiPitch >= minPitch && midiPitch < maxPitch) {
        noteMaps[midiPitch - minPitch] = std::make_unique<NoteState>(noteState);
    }
}
NoteState* SynthState::getNoteState (int midiPitch)
{
    if (midiPitch >= minPitch && midiPitch < maxPitch && noteMaps[midiPitch - minPitch]) {
        return noteMaps[midiPitch - minPitch].get();
    }
    return nullptr;
}

void SynthState::modifyGenIndex (int midiPitch, int delta)
{
    if (midiPitch < minPitch || midiPitch >= maxPitch) {
        return;
    }

    auto& note = noteMaps[midiPitch - minPitch];
    if ((int)(note->noteGenIndex) + delta >= 0) {
        note->noteGenIndex += delta;
        DBG( "modify note " << note->toString() << " with delta" << delta);
    }
}
void SynthState::setNoteBeingPlayed (int midiPitch, bool shouldBePlaying)
{
    if (midiPitch < minPitch || midiPitch >= maxPitch) {
        return;
    }
    notesBeingPlayed[midiPitch - minPitch] = shouldBePlaying;
}

bool SynthState::isNoteBeingPlayed (int midiPitch)
{
    if (midiPitch < minPitch || midiPitch >= maxPitch) {
        return false;
    }
    return notesBeingPlayed[midiPitch - minPitch];
}
