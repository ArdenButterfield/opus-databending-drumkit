//
// Created by arden on 4/30/24.
//

#include "SampleComparer.h"
#include "juce_core/juce_core.h"

SampleComparer::SampleComparer(unsigned int _length, float _sampleRate) : length(_length), sampleRate(_sampleRate), fft(juce::findHighestSetBit(length) + 1)
{

}

SampleComparer::~SampleComparer()
{

}

float SampleComparer::similarityScore(juce::AudioBuffer<float> a, juce::AudioBuffer<float> b)
{
    if (a.getNumSamples() != b.getNumSamples() || a.getNumChannels() != b.getNumChannels()) {
        DBG( "comparing samples with different dimensions... why?");
        return 0;
    }

    if (a.getNumSamples() > fft.getSize()) {
        DBG( "samples too big to compare with this comparer... you're the one who set the length here in the first place!");
        return 0;
    }

    if (a == b) {
        return 1;
    }

    auto aa = juce::AudioBuffer<float>(1, fft.getSize() * 2);
    auto bb = juce::AudioBuffer<float>(1, fft.getSize() * 2);

    aa.clear();
    bb.clear();

    aa.copyFrom(0,0,a,0,0,a.getNumSamples());
    bb.copyFrom(0,0,b,0,0,b.getNumSamples());

    fft.performFrequencyOnlyForwardTransform(aa.getWritePointer(0,0), true);
    fft.performFrequencyOnlyForwardTransform(bb.getWritePointer(0,0), true);

    std::vector<float> energyPerBand_a;
    std::vector<float> energyPerBand_b;
    std::vector<int> linesInBand;

    int line = 0;
    for (auto bandCutoff : barkHzCutoffs) {
        linesInBand.push_back(0);
        energyPerBand_a.push_back(0);
        energyPerBand_b.push_back(0);
        for (; lineToHerz(line) < bandCutoff; ++line) {
            linesInBand.back()++;
            energyPerBand_a.back() += aa.getSample(0,line);
            energyPerBand_b.back() += bb.getSample(0,line);
        }
        if (linesInBand.back() != 0) {
            energyPerBand_a.back() /= linesInBand.back();
            energyPerBand_b.back() /= linesInBand.back();
        }
    }

#if false
    for (auto v: energyPerBand_a) {
        std::cout << std::round(v) << "\t";
    }
    std::cout << "\n";
    for (auto v: energyPerBand_b) {
        std::cout << std::round(v) << "\t";
    }
    std::cout << "\n";
#endif

    auto dot_product = 0.f;

    for (unsigned int i = 0; i < energyPerBand_a.size(); ++i) {
        dot_product += energyPerBand_a[i] * energyPerBand_b[i];
    }

    auto square_magnitude_a = 0.f;
    for (auto v : energyPerBand_a) {
        square_magnitude_a += v * v;
    }

    auto square_magnitude_b = 0.f;
    for (auto v : energyPerBand_b) {
        square_magnitude_b += v * v;
    }
    auto denominator = (std::sqrt(square_magnitude_a) * std::sqrt(square_magnitude_b));
    return dot_product / denominator;
}
float SampleComparer::lineToHerz (const float line)
{
    return line * sampleRate / fft.getSize();
}
