//
// Created by arden on 4/23/24.
//

#include "Frontend.h"

Frontend::Frontend(SynthState& state) : synthState(state), noteGrid(synthState, 0, 127, 4)
{
    viewport.setViewedComponent(&noteGrid, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
}

Frontend::~Frontend() noexcept
{

}

void Frontend::resized()
{
    noteGrid.setSize(150, noteGrid.desiredHeight(150));
    viewport.setBounds(getLocalBounds().withWidth(150 + viewport.getScrollBarThickness()));
}

void Frontend::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::forestgreen);
}