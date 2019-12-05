/*
** Traveling salesman problem, 2019
** main.cpp
*/

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

#include "Graph.hpp"
#include "Population.hpp"

#ifdef TSP_GRAPHICS
	#include <SFML/System.hpp>
	#include <SFML/Window.hpp>
	#include <SFML/Graphics.hpp>
#endif

////////////////////////////////////////////////////////////////////////////////

static std::size_t generations    = 3000;
static std::size_t populationSize = 200;
static std::size_t eliteCount     = 40;
static double mutationRate        = 0.0005;

#ifdef TSP_GRAPHICS
	static float drawOffset = 50.0f;
	static float drawScale = 10;
#endif

////////////////////////////////////////////////////////////////////////////////

int main(int ac, char **av)
{
	assert(eliteCount <= populationSize);

	std::srand(std::time(NULL));

	if (ac != 2) {
		fprintf(stderr, "usage: %s <file.tsp>\n", av[0]);
		return EXIT_FAILURE;
	}

	TSP::Graph graph;
	if (!graph.loadFromFile(av[1])) {
		fprintf(stderr, "Failed to load graph from file\n");
		return EXIT_FAILURE;
	}


	TSP::Population pop(graph, populationSize, eliteCount, mutationRate);

#ifndef TSP_GRAPHICS
	for (size_t i = 1; i <= generations; ++i) {
		pop.crossAndMutate();
		pop.evaluateFitness();
		printf("Best is %.3f in generation #%zu\r", pop.best().distance(), i); fflush(stdout);
	}
	printf("\n");
#else
	sf::CircleShape dot(4.0f, 4);
	dot.setFillColor(sf::Color::Red);
	dot.setOrigin(3, 3);

	sf::Font consolas; consolas.loadFromFile("res/Consolas.ttf");
	sf::Text debugText("gen: ", consolas, 20);

	sf::RenderWindow window(sf::VideoMode(graph.max().x * drawScale + drawOffset * 2, graph.max().y * drawScale + drawOffset * 2), "Traveling salesman problem");
	sf::RenderStates states(sf::Transform().translate(drawOffset, drawOffset));

	std::size_t generation = 0;
	bool paused = false;
	for (bool running = true; window.isOpen() && running; )
	{
		sf::Event e;
		while (window.pollEvent(e)) {
			switch (e.type) {
				case sf::Event::Closed:
					running = false;
					break;

				case sf::Event::KeyPressed:
					if (e.key.code == sf::Keyboard::Escape)
						running = false;
					else if (e.key.code == sf::Keyboard::Space)
						paused = !paused;
					break;

				default:
					break;
			}
		}

		if (!paused) {
			generation++;
			pop.crossAndMutate();
			pop.evaluateFitness();
			printf("G %zu best: %f\n", generation, pop.best().distance());
			debugText.setString("G: " + std::to_string(generation) + "\nB: " + std::to_string(pop.best().distance()));

			if (generation == generations)
				paused = true;
		}

		window.clear(sf::Color::Black);
		// Draw cities
		for (auto &n : graph.nodes()) {
			dot.setPosition((float)n.x * drawScale, (float)n.y * drawScale);
			window.draw(dot, states);
		}
		// Draw routes
		auto &best = pop.best();
		for (size_t i = 1; i < best.genes().size(); ++i) {
			sf::Vertex line[2] = {
				sf::Vertex( sf::Vector2f(best.gene(i - 1).x, best.gene(i - 1).y) * drawScale ),
				sf::Vertex( sf::Vector2f(best.gene(i).x, best.gene(i).y) * drawScale )
			};
			window.draw(line, 2, sf::Lines, states);
		}
		window.draw(debugText);
		window.display();
	}

	if (window.isOpen())
		window.close();
#endif

	return EXIT_SUCCESS;
}