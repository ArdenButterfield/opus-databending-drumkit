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
    synthState.addNoteDefault(homeSlot->midiNote);

    addAndMakeVisible(previousSound);
    addAndMakeVisible(nextSound);
}
void NoteInGrid::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightskyblue);
    g.setColour(juce::Colours::darkslateblue);
    g.drawText(juce::String(synthState.getNoteState(homeSlot->midiNote)->noteGenIndex),
        getLocalBounds().withHeight(getHeight() / 2),
        juce::Justification::centred);
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
