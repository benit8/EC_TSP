/*
** TSP, 2019
** Population.cpp
*/

#include "Population.hpp"

#include <algorithm>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////

std::random_device g_rng;
double dRand(double max)
{
	return ((double)g_rng() / (float)g_rng.max()) * max;
}

double dRand(double min, double max)
{
	double range = max - min;
	double div = double(g_rng.max()) / range;
	return min + ((double)g_rng() / div);
}

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

////////////////////////////////////////////////////////////////////////////////

Population::Population(Graph &graph, std::size_t popSize, std::size_t eliteCount, double mutationRate)
: m_graph(graph)
, m_popSize(popSize)
, m_eliteCount(eliteCount)
, m_mutationRate(mutationRate)
{
	m_individuals.reserve(m_popSize);

	for (size_t i = 0; i < m_popSize; ++i)
		m_individuals.push_back(Individual(m_graph.nodes()));

	evaluateFitness();
}

////////////////////////////////////////////////////////////////////////////////

void Population::evaluateFitness()
{
	std::sort(m_individuals.begin(), m_individuals.end(), [] (const Individual &a, const Individual &b) {
		return a.distance() < b.distance();
	});

	// double sum = 0;
	// for (auto &idv : m_individuals) {
	// 	sum += idv.distance();
	// 	idv.m_cumSum = sum;
	// }

	// for (auto &idv : m_individuals)
	// 	idv.m_cumPerc = 100 * idv.m_cumSum / sum;

	// for (auto &idv : m_individuals) {
	// 	printf("%.15f (%.0f) [%.15f %.2f] %f\n",
	// 		idv.m_fitness, idv.m_distance, idv.m_cumSum, idv.m_cumPerc,
	// 		(1.0 / (double)m_popSize) - m_popSize * ((idv.distance() - best().distance()) / (best().distance() - worst().distance()))
	// 	);
	// }
}

void Population::crossAndMutate()
{
	// Individuals not in the elite range are resetted with random ones
	std::vector<Individual> matingPool(m_individuals.begin(), m_individuals.begin() + (m_eliteCount * 1.1));
	std::shuffle(matingPool.begin(), matingPool.end(), g_rng);

	std::vector<Individual> children;
	for (size_t i = 0; i < m_eliteCount; i += 2) {
		auto &parent1 = matingPool.at(i);
		auto &parent2 = matingPool.at(i + 1);

		auto child = parent1.cross(parent2);
		child.mutate(m_mutationRate);
		matingPool.push_back(child);
	}

	while (matingPool.size() < 200)
		matingPool.emplace_back(m_graph.nodes());

	m_individuals = matingPool;
}

// std::vector<Population::Individual> Population::buildMatingPool()
// {
// 	std::vector<Population::Individual> matingPool(
// 		m_individuals.begin(),
// 		m_individuals.begin() + m_eliteCount
// 	);

// 	for (size_t i = 0; i < m_popSize - m_eliteCount; ++i) {
// 		double pick = dRand(100);
// 		for (auto &idv : m_individuals) {
// 			if (pick <= idv.m_cumPerc) {
// 				matingPool.push_back(idv);
// 				break;
// 			}
// 		}
// 	}

// 	return matingPool;
// }

////////////////////////////////////////////////////////////////////////////////

}