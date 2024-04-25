//
// Created by arden on 4/23/24.
//

#include "Frontend.h"

Frontend::Frontend(SynthState& state) : synthState(state), noteGrid(synthState, 0, 127, 4), monitorChangingSamplesButton("monitor")
{
    viewport.setViewedComponent(&noteGrid, false);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);

    addAndMakeVisible(monitorChangingSamplesButton);
    monitorChangingSamplesButton.addListener(this);
}

Frontend::~Frontend() noexcept
{

}

void Frontend::resized()
{
    noteGrid.setSize(150, noteGrid.desiredHeight(150));
    viewport.setBounds(getLocalBounds().withWidth(150 + viewport.getScrollBarThickness()));
    monitorChangingSamplesButton.setBounds(150 + viewport.getScrollBarThickness(), 0, 200, 30);
}

void Frontend::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::forestgreen);
}
void Frontend::buttonStateChanged (juce::Button* button)
{
}
void Frontend::buttonClicked (juce::Button* button)
{
    if (button == &monitorChangingSamplesButton) {
        synthState.monitorSwitchingSamples = monitorChangingSamplesButton.getToggleState();
    }

}
void Frontend::timerCallback()
{
    monitorChangingSamplesButton.setToggleState(synthState.monitorSwitchingSamples, juce::dontSendNotification);
}
