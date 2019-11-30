/*
** TSP, 2019
** Graph.Node.cpp
*/

#include "Graph.hpp"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

namespace TSP
{

////////////////////////////////////////////////////////////////////////////////

double Graph::Node::dist(const Node &other) const
{
	auto pow2 = [] (double x) -> double {
		return x * x;
	};

	return sqrt(pow2(abs(other.x - x)) + pow2(abs(other.y - y)));
}

Graph::Node &Graph::Node::operator =(const Node &other)
{
	this->id = other.id;
	this->x = other.x;
	this->y = other.y;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}