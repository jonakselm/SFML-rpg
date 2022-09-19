#include "stdafx.h"
#include "Map.hpp"
#include <iostream>

Map::Map()
{
}

Map::~Map()
{
}

bool Map::load(const std::string &filename, const sf::Vector2u &windowSize)
{
	Json::Value root;

	std::ifstream file(filename);

	file >> root;

	mapsize.x = root["width"].asInt();
	mapsize.y = root["height"].asInt();

	tilesize.x = root["tilewidth"].asInt();
	tilesize.y = root["tileheight"].asInt();

	m_texture.create(mapsize.x * tilesize.x, mapsize.y * tilesize.y);
	m_mapSprite.setTexture(m_texture.getTexture());
	m_minimapSprite.setTexture(m_texture.getTexture());
	m_minimapSprite.setColor(sf::Color(255, 255, 255, 200));

	auto textureSize = sf::Vector2f(m_texture.getTexture().getSize());
	std::cout << "Created texture's size: " << textureSize.x << "x" << textureSize.y << std::endl;

	m_gameView.reset(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(windowSize)));
	m_minimapView.reset(sf::FloatRect(sf::Vector2f(0, 0), textureSize));

	m_gameView.setViewport(sf::FloatRect(0, 0, 1, 1));
	m_minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

	m_time = m_clock.restart();
	loadTileset(root);
	m_time = m_clock.restart();

	std::cout << "Loading tileset: " << m_time.asSeconds() << "s" << std::endl;

	loadLayer(root);
	m_time = m_clock.restart();

	std::cout << "Loading layers: " << m_time.asSeconds() << "s" << std::endl;

	loadTiles();
	m_time = m_clock.restart();

	std::cout << "Loading tiles: " << m_time.asSeconds() << "s" << std::endl;

	return true;
}

void Map::update(const sf::Time &elapsedTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_gameView.move({ 0, -1 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_gameView.move({ 0, 1 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_gameView.move({ -1, 0 });
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_gameView.move({ 1, 0 });

	m_texture.clear();
	for (const auto &tile : m_tiles)
	{
		tile->update();
		m_texture.draw(*tile);
	}
	m_texture.display();
}

void Map::addPlayer(const Player *pPlayer)
{
	m_player = pPlayer;
}

void Map::draw(sf::RenderTarget &target) const
{
	target.setView(m_gameView);
	target.draw(m_mapSprite);
	
	target.setView(m_minimapView);
	target.draw(m_minimapSprite);
}

void Map::loadTileset(const Json::Value &root)
{
	for (auto &val : root["tilesets"])
	{
		// Tilebased embedded
		if (!val["image"].empty())
		{
			m_tilesets.emplace_back(std::make_unique<Tileset>());
			m_tilesets.back()->load(val);
			m_tilesets.back()->setFirstgid(val["firstgid"].asInt());
		}
		// Non-embedded
		else if (!val["source"].empty())
		{
			Json::Value newVal;
			std::string sourcePath = "data/maps/" + val["source"].asString();
			std::ifstream file(sourcePath);
			file >> newVal;


			m_tilesets.emplace_back(std::make_unique<Tileset>());

			// Tilebased
			if (newVal["grid"].empty())
			{
				m_tilesets.back()->load(newVal);
			}
			// Imagebased
			else
			{

			}
			m_tilesets.back()->setFirstgid(val["firstgid"].asInt());
		}
		// Imagebased embedded
		else if (!val["grid"].empty())
		{
		}
	}
}

void Map::loadLayer(const Json::Value &root, const std::string &layerGroup)
{
	for (const auto &val : root["layers"])
	{
		if (val["type"].asString() == "tilelayer")
		{
			m_layers.emplace_back(std::make_unique<Layer>());
			m_layers.back()->loadTiles(val);
		}
		else if (val["type"].asString() == "objectgroup")
		{
		}
		else if (val["type"].asString() == "group")
		{
			loadLayer(val, val["name"].asString());
		}
	}
}

void Map::loadTiles()
{
	for (const auto &layer : m_layers)
	{
		for (int y = 0; y < layer->getSize().y; y++)
		{
			for (int x = 0; x < layer->getSize().x; x++)
			{
				int tileData = layer->getTile(sf::Vector2i(x, y));
				if (tileData != 0)
				{
					m_tiles.emplace_back(std::make_unique<TextureTile>());
					const Tileset &t = **std::find_if(m_tilesets.rbegin(), m_tilesets.rend(),
						[tileData](const std::unique_ptr<Tileset> &tileset)
						{
							return tileset->getFirstgid() <= tileData;
						});
					auto &tile = *m_tiles.back();
					tile.setTexture(t.getTexture());
					int localId = tileData - t.getFirstgid();
					sf::Vector2i tilesize = t.getTilesize();
					auto topLeft = sf::Vector2i(localId % t.getSize().x * tilesize.x,
						localId / t.getSize().x * tilesize.y);
					tile.setTextureRect(sf::IntRect(topLeft, tilesize));
					tile.setPosition(sf::Vector2f(x * tilesize.x, y * tilesize.y));
				}
			}
		}
	}
}
