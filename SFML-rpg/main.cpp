#include "stdafx.h"
#include "Map/Map.hpp"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");

	Map map;
	map.load("data/maps/test.json", window.getSize());

    sf::Clock clock;
    sf::Time time;

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
        time = clock.restart();
        map.update(time);

		window.clear();
        map.draw(window);
		window.display();

		std::cout << "Frame duration: " << time.asSeconds() << "s" << std::endl;
	}

	return 0;
}