//
// Created by arden on 4/23/24.
//

#include "NoteSlot.h"

NoteSlot::NoteSlot (SynthState& state, int _midiNote) : synthState(state), midiNote(_midiNote)
{
    startTimerHz(60);
}

NoteSlot::~NoteSlot() noexcept
{

}

void NoteSlot::paint (juce::Graphics& g)
{
    if (synthState.isNoteBeingPlayed(midiNote)) {
        g.fillAll(juce::Colours::yellow);
    }
    g.setColour(juce::Colours::darkslateblue);
    g.drawRect(getLocalBounds());
    g.drawText(juce::String(midiNote), getLocalBounds(), juce::Justification::centred);
}

void NoteSlot::resized()
{

}
void NoteSlot::mouseUp (const juce::MouseEvent& event)
{
    if (event.mouseWasClicked()) {
        note = std::make_unique<NoteInGrid>(synthState, this);
        addAndMakeVisible(note.get());
        note->setBounds(getLocalBounds());
    }
}
void NoteSlot::timerCallback()
{
    repaint();
}
