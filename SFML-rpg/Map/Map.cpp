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

	m_mapDetails.width = root["width"].asInt();
	m_mapDetails.height = root["height"].asInt();

	m_mapDetails.tileSize.x = root["tilewidth"].asInt();
	m_mapDetails.tileSize.y = root["tileheight"].asInt();

	m_texture.create(m_mapDetails.width * m_mapDetails.tileSize.x,
		m_mapDetails.height * m_mapDetails.tileSize.y);
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
	for (int i = 0; i < m_layers.size(); i++)
	{
		Layer *layer = nullptr;
		if (std::holds_alternative<TileLayer>(m_layers[i]))
			layer = &std::get<TileLayer>(m_layers[i]);
		else
			layer = &std::get<ObjectLayer>(m_layers[i]);
		layer->update(elapsedTime);
		layer->draw(m_texture);
		if (layer->group == "Entity")
		{
			if (m_pPlayer != nullptr)
				m_pPlayer->draw(m_texture);
		}
	}
	m_texture.display();
}

void Map::addPlayer(const Player *pPlayer)
{
	m_pPlayer = pPlayer;
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
	for (const auto &val : root["tilesets"])
	{
		// Tilebased embedded
		if (!val["image"].empty())
		{
			TilebasedTileset tileset;
			tileset.load(val);
			m_tilesets.emplace_back().emplace<const TilebasedTileset>(tileset);
		}
		// Non-embedded
		else if (!val["source"].empty())
		{
			Json::Value newVal;
			std::string sourcePath = "data/maps/" + val["source"].asString();
			std::ifstream file(sourcePath);
			file >> newVal;

			// Tilebased
			if (newVal["grid"].empty())
			{
				TilebasedTileset tileset;
				tileset.load(newVal);
				tileset.firstgid = val["firstgid"].asInt();
				m_tilesets.emplace_back().emplace<const TilebasedTileset>(tileset);
			}
			// Imagebased
			else
			{
				ImageTileset tileset;
				tileset.load(newVal);
				tileset.firstgid = val["firstgid"].asInt();
				m_tilesets.emplace_back().emplace<const ImageTileset>(tileset);

			}
		}
		// Imagebased embedded
		else if (!val["grid"].empty())
		{
			ImageTileset tileset;
			tileset.load(val);
			m_tilesets.emplace_back().emplace<const ImageTileset>(tileset);
		}
	}
}

void Map::loadLayer(const Json::Value &root, const std::string &layerGroup)
{
	for (const auto &val : root["layers"])
	{
		if (val["type"].asString() == "tilelayer")
		{
			TileLayer layer;
			layer.load(val, layerGroup, m_tilesets, m_mapDetails);
			m_layers.emplace_back().emplace<TileLayer>(layer);
		}
		else if (val["type"].asString() == "objectgroup")
		{
			ObjectLayer layer;
			layer.load(val, layerGroup, m_tilesets, m_mapDetails);
			m_layers.emplace_back().emplace<ObjectLayer>(layer);
		}
		else if (val["type"].asString() == "group")
		{
			loadLayer(val, val["name"].asString());
		}
	}
}
