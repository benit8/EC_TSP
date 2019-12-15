/*
** TSP, 2019
** Population.Individual.cpp
*/

#include "Population.hpp"

#include <algorithm>
#include <cassert>

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

////////////////////////////////////////////////////////////////////////////////

Population::Individual::Individual(const std::vector<Graph::Node> &nodes)
: m_genes(nodes)
{
	std::shuffle(m_genes.begin(), m_genes.end(), g_rng);
	computeFitness();
}

////////////////////////////////////////////////////////////////////////////////

void Population::Individual::computeFitness()
{
	for (std::size_t i = 1; i < m_genes.size(); ++i)
		m_distance += m_genes[i - 1].dist(m_genes[i]);
	m_distance += m_genes.rbegin()->dist(m_genes[0]);

	m_fitness = 1.0 / m_distance;
}

Population::Individual Population::Individual::cross(const Individual &other)
{
	assert(m_genes.size() == other.genes().size());
	std::size_t genesCount = m_genes.size();

	// Select a range of genes from this parent
	std::size_t start  = g_rng() % genesCount;
	std::size_t length = g_rng() % (genesCount - start);

	// Copy the range of genes from this parent from the child
	Individual child;
	child.m_genes.resize(genesCount);
	std::copy(m_genes.begin() + start, m_genes.begin() + start + length, child.m_genes.begin());

	// Copy the other parent's genes, if they're not already in the child
	std::size_t i = length;
	for (auto &g : other.genes()) {
		assert(g.id != 0);

		auto it = std::find(child.genes().begin(), child.genes().end(), g);
		if (it == child.genes().end())
			child.m_genes[i++] = g;
	}

	child.computeFitness();
	return child;
}

void Population::Individual::mutate(double mutationRate)
{
	const size_t size = m_genes.size();
	for (size_t i = 0; i < size; ++i) {
		if (dRand() < mutationRate) {
			// std::swap(m_genes[i], m_genes[g_rng() % size]);
			// auto start = m_genes.begin() + i;
			// auto end = start + (g_rng() % (size - i));
			// std::reverse(start, end);
			size_t len = g_rng() % size;
			for (size_t start = i, end = i + len; start < end; ++start, --end) {
				std::swap(m_genes.at(start % size), m_genes.at(end % size));
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

}