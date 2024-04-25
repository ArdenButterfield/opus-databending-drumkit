//
// Created by del on 4/23/24.
//

#ifndef OPUSDATABENDINGDRUMKIT_GENETICEVOLVER_H
#define OPUSDATABENDINGDRUMKIT_GENETICEVOLVER_H

#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>

class GeneticIndividual
{
public:
    GeneticIndividual(std::vector<unsigned char> genome);
    // [0, 1]
    // lazy
    float getHeuristic();
    float calcHeuristic();

    std::vector<unsigned char> getGenome();

    friend bool operator<(GeneticIndividual l, GeneticIndividual r);
private:
    std::vector<unsigned char> genome;
    std::optional<float> heuristicResult;
};

class GeneticEvolver
{
public:
    GeneticEvolver(int64_t randomSeed);

    unsigned char mutateChromosome(unsigned char c);

    GeneticIndividual mate(GeneticIndividual l, GeneticIndividual r);

    std::vector<GeneticIndividual> evolve(std::vector<GeneticIndividual> ancestor);

private:
    int maxGenerations;
    juce::Random random;
};

#endif //OPUSDATABENDINGDRUMKIT_GENETICEVOLVER_H
