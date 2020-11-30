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

	m_mapSize.x = root["width"].asInt();
	m_mapSize.y = root["height"].asInt();

	m_tileSize.x = root["tilewidth"].asInt();
	m_tileSize.y = root["tileheight"].asInt();

	m_texture.create(m_mapSize.x * m_tileSize.x, m_mapSize.y * m_tileSize.y);
	m_sprite.setTexture(m_texture.getTexture());

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

	return true;
}

void Map::update(const sf::Time &elapsedTime)
{
	m_texture.clear();
	for (auto &layer : m_layers)
	{
		layer->update(elapsedTime);
		layer->draw(m_texture);
	}
	m_texture.display();
}

void Map::draw(sf::RenderTarget &target) const
{
	target.setView(m_gameView);
	target.draw(m_sprite);
	
	target.setView(m_minimapView);
	target.draw(m_sprite);
}

void Map::loadTileset(const Json::Value &root)
{
	for (const auto &val : root["tilesets"])
	{
		if (!val["image"].empty())
		{
			std::unique_ptr<TilebasedTileset> pTileset;
			pTileset->load(val);

			m_tilesets.push_back(std::move(pTileset));
		}
		else if (!val["source"].empty())
		{
			Json::Value newVal;
			std::string sourcePath = "data/maps/" + val["source"].asString();

			std::ifstream file(sourcePath);

			file >> newVal;

			if (newVal["grid"].empty())
			{
				std::unique_ptr<TilebasedTileset> pTileset;
				pTileset->load(newVal);
				pTileset->firstgid = val["firstgid"].asInt();

				m_tilesets.push_back(std::move(pTileset));
			}
			else
			{
				std::unique_ptr<ImageTileset> pTileset;
				pTileset->load(newVal);
				pTileset->firstgid = val["firstgid"].asInt();

				m_tilesets.push_back(std::move(pTileset));
			}
		}
		else if (!val["grid"].empty())
		{
			std::unique_ptr<ImageTileset> pTileset;
			pTileset->load(val);

			m_tilesets.push_back(std::move(pTileset));
		}
	}
}

void Map::loadLayer(const Json::Value &root, const std::string &layerGroup)
{
	for (const auto &val : root["layers"])
	{
		if (val["type"].asString() == "tilelayer")
		{
			std::unique_ptr<TileLayer> pLayer;
			pLayer->load(val, layerGroup, m_tilesets, m_mapSize);
			m_layers.push_back(std::move(pLayer));
		}
		else if (val["type"].asString() == "objectgroup")
		{
			if (val["name"].asString() == "EntityLayer")
			{

			}
			else
			{
				std::unique_ptr<ObjectLayer> pLayer;
				pLayer->load(val, layerGroup, m_tilesets, m_mapSize);
				m_layers.push_back(std::move(pLayer));
			}
		}
		else if (val["type"].asString() == "group")
		{
			loadLayer(val, val["name"].asString());
		}
	}
}
