/*
** TSP, 2019
** Graph.cpp
*/

#include "Graph.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

////////////////////////////////////////////////////////////////////////////////

bool Graph::loadFromFile(const std::string &filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		return false;

	for (std::string lineType; ifs; ifs.ignore(1L << 12, '\n')) {
		ifs >> lineType;
		// Just advance until we found the coordinates section
		if (lineType == "NODE_COORD_SECTION")
			break;
	}

	ifs.ignore(1L << 12, '\n');

	if (ifs.eof()) {
		fprintf(stderr, "TSP::Graph::loadFromFile: No data in file\n");
		return false;
	}

	for (std::string line; std::getline(ifs, line); ) {
		if (line[line.length() - 1] == '\r')
			line.pop_back();

		if (line == "EOF")
			break;

		std::istringstream iss(line);
		Node node;
		if (iss >> node.id >> node.x >> node.y) {
			if (node.x > m_max.x) m_max.x = node.x;
			if (node.y > m_max.y) m_max.y = node.y;
			m_nodes.push_back(node);
		}
		else {
			fprintf(stderr, "TSP::Graph::loadFromFile: Failed to parse node data\n");
			return false;
		}
	}

	return true;
}

const Graph::Node &Graph::node(std::size_t id) const
{
	assert(id > 0 && id < m_nodes.size());

	auto it = std::find_if(m_nodes.begin(), m_nodes.end(), [&id] (const Node &n) {
		return n.id == id;
	});

	assert(it != m_nodes.end());
	return *it;
}

////////////////////////////////////////////////////////////////////////////////

}