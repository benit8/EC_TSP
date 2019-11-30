/*
** TSP, 2019
** Graph.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace TSP {
	class Graph;
}

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

class Graph
{
public:
	struct Node
	{
		std::size_t id = 0;
		unsigned x = 0;
		unsigned y = 0;

		double dist(const Node &other) const;

		Node &operator =(const Node &other);
		bool operator ==(const Node &other) const { return id == other.id; }
		bool operator !=(const Node &other) const { return id != other.id; }
	};

public:
	Graph() = default;
	~Graph() = default;

	bool loadFromFile(const std::string &filename);
	const Node &node(std::size_t id) const;

	const std::vector<Node> &nodes() const { return m_nodes; }
	const Node &max() const { return m_max; }

private:
	std::vector<Node> m_nodes;
	Node m_max;
};

}