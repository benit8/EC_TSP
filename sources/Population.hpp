/*
** TSP, 2019
** Population.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace TSP {
	class Population;
}

////////////////////////////////////////////////////////////////////////////////

#include "Graph.hpp"

#include <vector>
#include <random>

////////////////////////////////////////////////////////////////////////////////

extern std::random_device g_rng;
extern double dRand(double max = 1.0);
extern double dRand(double min, double max);

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

////////////////////////////////////////////////////////////////////////////////

class Population
{
public:
	class Individual
	{
	public:
		Individual(const std::vector<Graph::Node> &nodes);

		void computeFitness();
		Individual cross(const Individual &other);
		void mutate(double mutationRate);

		const std::vector<Graph::Node> &genes() const { return m_genes; }
		const Graph::Node &gene(std::size_t i) const { return m_genes[i]; }
		Graph::Node &gene(std::size_t i) { return m_genes[i]; }
		double fitness() const { return m_fitness; }
		double distance() const { return m_distance; }

	private:
		Individual() = default;

	private:
		std::vector<Graph::Node> m_genes;
		double m_distance = 0;
		double m_fitness = 0;
		double m_cumSum = 0;
		double m_cumPerc = 0;

		friend class Population;
	};

public:
	Population(Graph &graph, std::size_t popSize, std::size_t eliteCount, double mutationRate);

	void evaluateFitness();
	void crossAndMutate();

	const std::vector<Individual> &individuals() const { return m_individuals; }
	const Individual &best() const { return m_individuals.front(); }
	const Individual &worst() const { return m_individuals.back(); }

// private:
// 	std::vector<Individual> buildMatingPool();

private:
	Graph &m_graph;
	std::size_t m_popSize;
	std::size_t m_eliteCount;
	double m_mutationRate;
	std::vector<Individual> m_individuals;
};

////////////////////////////////////////////////////////////////////////////////

}