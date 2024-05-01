//
// Created by arden on 4/30/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_SAMPLECOMPARER_H
#define OPUSDATABENDINGDRUMKIT_SAMPLECOMPARER_H

#include "juce_dsp/juce_dsp.h"
#include <vector>

class SampleComparer
{
public:
    SampleComparer(unsigned int length, float sampleRate);
    ~SampleComparer();

    // 1 = identical 0 = no correlation
    float similarityScore(juce::AudioBuffer<float> a, juce::AudioBuffer<float> b);
private:
    float lineToHerz(const float line);

    const std::vector<float> barkHzCutoffs {
        100,
        200,
        300,
        400,
        510,
        630,
        770,
        920,
        1080,
        1270,
        1480,
        1720,
        2000,
        2300,
        2700,
        3150,
        3700,
        4400,
        5300,
        6400,
        7700,
        9500,
        12000,
        20000
    };

    const unsigned int length;
    const float sampleRate;
    juce::dsp::FFT fft;
};

#endif //OPUSDATABENDINGDRUMKIT_SAMPLECOMPARER_H
