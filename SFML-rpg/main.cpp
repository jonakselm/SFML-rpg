#include "stdafx.h"
#include "Map.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");

	Map map;
	map.load("data/maps/test.json");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
					break;
			}
		}
		window.clear();
		map.draw(window);
		window.display();
	}

	return 0;
}