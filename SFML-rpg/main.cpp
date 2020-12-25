#include "stdafx.h"
#include "Map/Map.hpp"
#include <iostream>
#include <Windows.h>
#include "Entity/Player.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");

	Map map;
	map.load("data/maps/someMap.json", window.getSize());

	Player player;

	map.addPlayer(&player);

	/*Json::Value tilesetRoot;
	std::ifstream tilesetFile("data/maps/mountain_landscape.json");
	tilesetFile >> tilesetRoot;
	TilebasedTileset tileset;
	tileset.load(tilesetRoot);
	Json::Value objectRoot;
	std::ifstream objectFile("data/maps/templates/rock.json");
	objectFile >> objectRoot;
	TemplateObject object;
	object.load(objectRoot, tileset);

	TextureTile tt;
	tt.load(object, 1);*/
	/*tt.m_transformable.setOrigin(0, 0);

	tt.m_vertices[0].position = sf::Vector2f(0, 0);
	tt.m_vertices[1].position = sf::Vector2f(32, 0);
	tt.m_vertices[2].position = sf::Vector2f(32, 32);
	tt.m_vertices[3].position = sf::Vector2f(0, 32);

	tt.m_transformable.setPosition(100, 100);*/
	
	//tt.m_renderStates.texture = &tileset.texture;
	/*tt.m_renderStates.transform = tt.m_transformable.getTransform();

	auto rect = object.objProps.rect;

	tt.m_vertices[0].texCoords = sf::Vector2f(rect.left, rect.top);
	tt.m_vertices[1].texCoords = sf::Vector2f(rect.width, rect.top);
	tt.m_vertices[2].texCoords = sf::Vector2f(rect.width, rect.height);
	tt.m_vertices[3].texCoords = sf::Vector2f(rect.left, rect.height);
	tt.m_vertices[0].color = sf::Color(255, 255, 255, 255);
	tt.m_vertices[1].color = sf::Color(255, 255, 255, 128);
	tt.m_vertices[2].color = sf::Color(255, 255, 255, 128);
	tt.m_vertices[3].color = sf::Color(255, 255, 255, 0);*/

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

		std::cout << "Frame duration: " << time.asSeconds() << "s" << std::endl;
		std::wstring wstr = std::to_wstring(1000000 / time.asMicroseconds());
		SetWindowText(window.getSystemHandle(), wstr.c_str());
	}

	float averageTime = totalElapsedTime.asSeconds() / iterations;
	std::cout << "Average frame duration: " << averageTime << std::endl;
	std::cout << "Average FPS: " << 1.f / averageTime << std::endl;

	return 0;
}