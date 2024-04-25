//
// Created by del on 4/23/24.
//

#include "GeneticEvolver.h"

#include <utility>
#include <queue>

/*
 * ==== GeneticIndividual ====
 */

GeneticIndividual::GeneticIndividual (std::vector<unsigned char> g)
{
    this->genome = std::move(g);
}

float GeneticIndividual::calcHeuristic()
{
    // TODO: impl
    return 0;
}

float GeneticIndividual::getHeuristic()
{
    if(!this->heuristicResult.has_value()) {
        this->heuristicResult = this->calcHeuristic();
    }
    return this->heuristicResult.value();
}

std::vector<unsigned char> GeneticIndividual::getGenome()
{
    return this->genome;
}

bool operator< (GeneticIndividual l, GeneticIndividual r)
{
    return l.getHeuristic() < r.getHeuristic();
}

/*
 * ==== GeneticEvolver ====
 */

GeneticEvolver::GeneticEvolver (int64_t randomSeed)
{
    this->random = juce::Random(randomSeed);
    this->random.setSeed(randomSeed);
    this->maxGenerations = 32; // TODO: tune maxGenerations
}

unsigned char GeneticEvolver::mutateChromosome (unsigned char c)
{
    // 50% chance to flip any given bit
    auto to_flip = static_cast<unsigned char>(this->random.nextInt(256));
    return c ^ to_flip;
}

GeneticIndividual GeneticEvolver::mate (GeneticIndividual l, GeneticIndividual r)
{
    const float p_mutate   = 0.1f;
    const float p_parent_r = 0.5f; // p_parent_l = 1 - p_parent_r

    auto size = l.getGenome().size();
    auto genome = std::vector<unsigned char>(l.getGenome());

    for(unsigned long i = 0; i < size; i++) {
        float whomst = this->random.nextFloat();
        float mutate = this->random.nextFloat();

        if(whomst < p_parent_r) {
            genome[i] = r.getGenome()[i];
        } // else stays as parent l

        if(mutate < p_mutate) {
            genome[i] = this->mutateChromosome(genome[i]);
        }
    }

    return GeneticIndividual(genome);
}

std::vector<GeneticIndividual> GeneticEvolver::evolve (std::vector<GeneticIndividual> ancestors)
{
    const float p_survive    = 0.1f;
    const float p_mate       = 0.5f;
    const int   n_population = 32;

    // TODO: should ancestors be mutated or should we begin with a homogenous population?
    auto population = std::vector<GeneticIndividual>(std::move(ancestors));

    for(int i_gen = 0; i_gen < this->maxGenerations; i_gen++) {
        std::vector<GeneticIndividual> next_gen;

        auto n_survive = static_cast<int>(p_survive * n_population);
        auto n_mate    = static_cast<int>(p_mate * n_population);

        for(int i = 0; i < n_mate; i++) {
            auto individual = population[static_cast<unsigned long>(i)];

            if(i < n_survive) {
                next_gen.push_back(individual);
            }

            // mate with another member within first n_mate individuals (might be self)
            auto i_mate = static_cast<unsigned long>(this->random.nextInt(n_mate));
            auto partner = population[i_mate];

            auto offspring = this->mate(individual, partner);
            next_gen.push_back(std::move(offspring));
        }

        population = std::move(next_gen);
        std::sort(population.begin(), population.end());
    }

    return population;
}