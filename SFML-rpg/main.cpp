#include "stdafx.h"
#include "Map/Map.hpp"
#include <iostream>
#include <Windows.h>
#include "Entity/Player.hpp"

int main()
{
	// TODO: Use chunks, because of efficiency
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");

	std::wstring str;

	Map map;
	map.load("data/maps/test.json", window.getSize());

	Player player;

	map.addPlayer(&player);

    sf::Clock clock;
    sf::Time time;
	sf::Time totalElapsedTime;
	int iterations = 0;
	float speed = 64.f;

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
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					player.move(0, -speed * time.asSeconds());
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					player.move(0, speed * time.asSeconds());
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					player.move(-speed * time.asSeconds(), 0);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					player.move(speed * time.asSeconds(), 0);
				break;
			}
		}
        time = clock.restart();
        map.update(time);
		totalElapsedTime += time;
		iterations++;

		window.clear();
        map.draw(window);
		window.display();
	}

	float averageTime = totalElapsedTime.asSeconds() / iterations;
	std::cout << "Average frame duration: " << averageTime << std::endl;
	std::cout << "Average FPS: " << 1.f / averageTime << std::endl;

	return 0;
}