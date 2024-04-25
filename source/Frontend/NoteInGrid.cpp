//
// Created by arden on 4/23/24.
//

#include "NoteInGrid.h"
#include "NoteSlot.h"

NoteInGrid::NoteInGrid(SynthState& state, NoteSlot* slot)
    : homeSlot(slot),
      synthState(state),
      previousSound("Previous", 0.5, juce::Colours::darkviolet),
      nextSound("Next", 0.0, juce::Colours::darkviolet)
{
    startTimerHz(60);
    synthState.addNoteDefault(homeSlot->midiNote);

    addAndMakeVisible(previousSound);
    addAndMakeVisible(nextSound);

    previousSound.addListener(this);
    nextSound.addListener(this);
}
void NoteInGrid::paint (juce::Graphics& g)
{

    if (synthState.isNoteBeingPlayed(homeSlot->midiNote)) {
        g.fillAll(juce::Colours::yellow);
    } else {
        g.fillAll(juce::Colours::lightskyblue);
    }
    g.setColour(juce::Colours::darkslateblue);
    const auto s = synthState.getNoteState(homeSlot->midiNote);
    if (s != nullptr) {
        g.drawText(juce::String(s->noteGenIndex),
                   getLocalBounds().withHeight(getHeight() / 2),
                   juce::Justification::centred);
    }
}

void NoteInGrid::resized()
{
    auto bottomPart = getLocalBounds().withTrimmedTop(getHeight() / 2);
    previousSound.setBounds(bottomPart.withWidth(bottomPart.getWidth() / 2));
    nextSound.setBounds(previousSound.getBounds().withRightX(getRight()));
}
void NoteInGrid::buttonClicked (juce::Button* button)
{
    if (button == &previousSound) {
        synthState.modifyGenIndex(homeSlot->midiNote, -1);
    } else if (button == &nextSound) {
        synthState.modifyGenIndex(homeSlot->midiNote, 1);
    }
}

void NoteInGrid::buttonStateChanged (juce::Button* button)
{

}
void NoteInGrid::timerCallback()
{
    repaint();
}
