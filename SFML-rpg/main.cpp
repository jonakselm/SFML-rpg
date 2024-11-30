#include "stdafx.h"
#include "Map/Map.hpp"
#include <iostream>
//#include <Windows.h>
#include "Entity/Player.hpp"
#include <filesystem>

int main()
{
	try
	{
		// TODO: Use chunks, because of efficiency
		sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");
		window.setFramerateLimit(120);

		std::wstring str;
		std::filesystem::path sourcePath = std::filesystem::current_path();
		while (true)
		{
			auto testPath = sourcePath/"SFML-rpg";
			if (std::filesystem::exists(testPath) && std::filesystem::is_directory(testPath))
			{
				sourcePath = testPath;
				break;
			}
			sourcePath = sourcePath.parent_path();
			std::cout << sourcePath << std::endl;
		}

		Map map;
		map.load(sourcePath, "data/maps/test.json", window.getSize());

		Player player;

		map.addPlayer(&player);

		sf::Clock clock;
		sf::Time time;
		sf::Time totalElapsedTime;
		int iterations = 0;
		float speed = 32.f;

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
					if (//sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
						sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
						player.move(-speed, 0);
					else if (//sf::Keyboard::isKeyPressed(sf::Keyboard::F) ||
						sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
						player.move(speed, 0);
					else
						if (//sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
							sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
							player.move(0, -speed);
						else if (//sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
							sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
							player.move(0, speed);
					break;
				}
			}
    	    time = clock.restart();
    	    map.update(time);
			player.update(time);
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
	catch (const std::exception &e)
	{
		std::cout << "Unhandled exception occurred: " << e.what() << std::endl;
		std::exit(-1);
	}
	catch (...)
	{
		std::cout << "Unknown unhandled exception occurred" << std::endl;
		std::exit(-1);
	}	
}
