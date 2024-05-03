//
// Created by arden on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SYNTHSTATE_H
#define OPUSDATABENDINGDRUMKIT_SYNTHSTATE_H

#include "juce_core/juce_core.h"

struct NoteState
{
    NoteState(juce::int64 _seed, unsigned int _noteGenIndex) {
        seed = _seed;
        noteGenIndex = _noteGenIndex;
    }

    NoteState(const NoteState& n)
    {
        seed = n.seed.load();
        noteGenIndex = n.noteGenIndex.load();
    }

    std::atomic<juce::int64> seed;
    std::atomic<unsigned int> noteGenIndex;

    [[nodiscard]] std::string toString() const
    {
        return std::to_string (seed) + ":" + std::to_string (noteGenIndex);
    }
};

namespace std
{
    template <>
    struct hash<NoteState>
    {
        std::size_t operator()(const NoteState& noteState) const
        {
            return hash<std::string>()(noteState.toString());
        }
    };
}


inline bool operator==(const NoteState& lhs, const NoteState& rhs)
{
    return lhs.seed == rhs.seed && lhs.noteGenIndex == rhs.noteGenIndex;
}

class SynthState
{
public:
    SynthState(int minPitch, int maxPitch, NoteState defaultNote);
    ~SynthState();

    void addNote(int midiPitch, const NoteState& noteState);
    void addNoteDefault(int midiPitch);
    NoteState* getNoteState(int midiPitch);
    void modifyGenIndex(int midiPitch, int delta);
    void setNoteBeingPlayed(int midiPitch, bool shouldBePlaying);
    bool isNoteBeingPlayed(int midiPitch);

    std::atomic<bool> monitorSwitchingSamples;

    std::atomic<int> midiPitchToMonitor;
private:
    int minPitch;
    int maxPitch;

    const NoteState defaultNote;
    std::vector<bool> notesBeingPlayed;
    std::vector<std::unique_ptr<NoteState>> noteMaps;
};

#endif //OPUSDATABENDINGDRUMKIT_SYNTHSTATE_H
