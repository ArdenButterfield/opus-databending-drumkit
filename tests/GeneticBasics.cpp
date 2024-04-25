//
// Created by del on 4/23/24.
//

#include <GeneticEvolver.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>

TEST_CASE ("GeneticIndividual heuristic", "[individual]")
{
    std::vector<unsigned char> genome = { 0, 255, 0, 255 };
    auto individual = GeneticIndividual(genome);

    SECTION ("heuristic")
    {
        CHECK_THAT (individual.getHeuristic(),
                    Catch::Matchers::WithinRel(individual.calcHeuristic()));
    }
}

TEST_CASE ("GeneticEvolver::mutateChromosome flips bits predictably", "[mutateChromosome]")
{
    auto evolver = GeneticEvolver(1312);

    SECTION ("bitflip")
    {
        REQUIRE(evolver.mutateChromosome(0) == 30);
    }
}

TEST_CASE ("GeneticEvolver::mate two individuals", "[mate]")
{
    std::vector<unsigned char> genome1 = { 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255 };
    std::vector<unsigned char> genome2 = { 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0 };
    auto individual1 = GeneticIndividual(genome1);
    auto individual2 = GeneticIndividual(genome2);

    auto evolver = GeneticEvolver(1312);

    auto offspring = evolver.mate(individual1, individual2);

    auto o_genome = offspring.getGenome();

//    CAPTURE(o_genome);

    SECTION ("unique_new_genome")
    {
        REQUIRE_THAT (offspring.getGenome(),
                    !Catch::Matchers::Equals(genome1) && !Catch::Matchers::Equals(genome2));
        REQUIRE(o_genome.size() == genome1.size());
    }
}