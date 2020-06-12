#include "stdafx.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML-rpg");

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
		window.display();
	}

	return 0;
}