#include "stdafx.h"
#include "Map.hpp"
#include <iostream>
#include <cmath>

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

	m_mapsize.x = root["width"].asInt();
	m_mapsize.y = root["height"].asInt();

	m_tilesize.x = root["tilewidth"].asInt();
	m_tilesize.y = root["tileheight"].asInt();

	auto mapBounds = sf::Vector2f(m_mapsize.x * m_tilesize.x, m_mapsize.y * m_tilesize.y);
	//m_texture.create(m_mapsize.x * m_tilesize.x, m_mapsize.y * m_tilesize.y);
	//m_mapSprite.setTexture(m_texture.getTexture());
	//m_minimapSprite.setTexture(m_texture.getTexture());
	//m_minimapSprite.setColor(sf::Color(255, 255, 255, 200));

	//auto textureSize = sf::Vector2f(m_texture.getTexture().getSize());
	//std::cout << "Created texture's size: " << textureSize.x << "x" << textureSize.y << std::endl;

	m_gameView.reset(sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(windowSize)));
	m_minimapView.reset(sf::FloatRect(sf::Vector2f(0, 0), mapBounds));

	m_gameView.setViewport(sf::FloatRect(0, 0, 1, 1));
	m_minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));

	// Sets the opacity of whatever uses this shader to 70%
	const std::string opacityShader = \
		"uniform sampler2D texture;" \
		"void main()" \
		"{" \
		"     vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
		"     gl_FragColor = gl_Color * pixel;" \
		"     gl_FragColor.a = 0.7;" \
		"}";
	if (!m_opacityShader.loadFromMemory(opacityShader, sf::Shader::Fragment))
	{
		exit(101);
	}

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

	//m_texture.clear();
	for (auto &chunk : m_chunks)
	{
		if (isInView(*chunk))
		{
			// TODO: Support for animations
			if (chunk->hasAnimation())
				chunk->update();
			//for (int i = 0; i < 10000; i++)
				//chunk->draw(m_texture);
		}
	}
	//m_texture.display();
}

void Map::addPlayer(const Player *pPlayer)
{
	m_player = pPlayer;
}

void Map::draw(sf::RenderTarget &target) const
{
	target.setView(m_gameView);
	for (int i = 0; i < m_chunks.size() / 2; i++)
		if (isInView(*m_chunks[i]))
			m_chunks[i]->draw(target);
	m_player->draw(target);
	for (int i = m_chunks.size() / 2; i < m_chunks.size(); i++)
		if (isInView(*m_chunks[i]))
			m_chunks[i]->draw(target);
	//target.draw(m_mapSprite);

	/*target.setView(m_minimapView);
	for (const auto &chunk : m_chunks)
		// Only for testing selective rendering
		//if (isInView(*chunk))
		chunk->draw(target, &m_opacityShader);*/
	//target.draw(m_minimapSprite);
}

void Map::loadTileset(const Json::Value &root)
{
	for (auto &val : root["tilesets"])
	{
		// Tilebased embedded
		if (!val["image"].empty())
		{
			m_tilesets.emplace_back();
			m_tilesets.back().load(val);
			m_tilesets.back().setFirstgid(val["firstgid"].asInt());
		}
		// Non-embedded
		else if (!val["source"].empty())
		{
			Json::Value newVal;
			std::string sourcePath = "data/maps/" + val["source"].asString();
			std::ifstream file(sourcePath);
			file >> newVal;


			m_tilesets.emplace_back();

			// Tilebased
			if (newVal["grid"].empty())
			{
				m_tilesets.back().load(newVal);
			}
			// Imagebased
			else
			{

			}
			m_tilesets.back().setFirstgid(val["firstgid"].asInt());
		}
		// Imagebased embedded
		else if (!val["grid"].empty())
		{
		}
	}
}

void Map::loadLayer(const Json::Value &root, const std::string &layerGroup)
{
	std::vector<Json::Value> objectLayers;
	for (const auto &val : root["layers"])
	{
		if (val["type"].asString() == "tilelayer")
		{
			m_layers.emplace_back();
			m_layers.back().loadTiles(val, layerGroup);
		}
		else if (val["type"].asString() == "objectgroup")
		{
			objectLayers.push_back(val);
		}
		else if (val["type"].asString() == "group")
		{
			loadLayer(val, val["name"].asString());
		}
	}
	for (const auto &val : objectLayers)
	{
		// TODO: Make objects from objectlayers
	}
}

void Map::loadTiles()
{
	// Name is not descriptive enough, but this means the amounts of chunks in each axis
	int chunksWidth = std::ceil(float(m_mapsize.x)) / m_chunkSize.x;
	int chunksHeight = (int)std::ceil(float(m_mapsize.y)) / m_chunkSize.y;
	for (int z = 0; z < 2; z++)
		for (int y = 0; y < chunksHeight; y++)
			for (int x = 0; x < chunksWidth; x++)
				m_chunks.emplace_back(std::make_unique<Chunk>(m_chunkSize, m_tilesize,
					sf::Vector2f(x, y)));
	for (const auto &layer : m_layers)
	{
		int chunkZ = 0;
		if (layer.getGroup() == "Walk Under")
			chunkZ = 1;
		int chunkY = 0;
		for (int y = 0; y < layer.getSize().y; y++)
		{
			int chunkX = 0;
			if (y * m_tilesize.y >= m_chunks[(size_t)chunkX + (size_t)chunkY * chunksWidth +
				(size_t)chunkZ * chunksWidth * chunksHeight]->getBottomRight().y)
				chunkY++;
			for (int x = 0; x < layer.getSize().x; x++)
			{
				if (x * m_tilesize.x >= m_chunks[(size_t)chunkX + (size_t)chunkY * chunksWidth +
					(size_t)chunkZ * chunksWidth * chunksHeight]->getBottomRight().x)
					chunkX++;
				int tileData = layer.getTile(sf::Vector2i(x, y));
				if (tileData != 0)
				{
					TextureTile tile;
					const Tileset &t = *std::find_if(m_tilesets.rbegin(), m_tilesets.rend(),
						[tileData](const Tileset &tileset)
						{
							return tileset.getFirstgid() <= tileData;
						});
					tile.setTexture(t.getTexture());
					int localId = tileData - t.getFirstgid();
					sf::Vector2i tilesize = t.getTilesize();
					auto topLeft = sf::Vector2i(localId % t.getSize().x * tilesize.x,
						localId / t.getSize().x * tilesize.y);
					tile.setTextureRect(sf::IntRect(topLeft, tilesize));
					Chunk &chunk = *m_chunks[(size_t)chunkX + (size_t)chunkY * chunksWidth +
						(size_t)chunkZ * chunksWidth * chunksHeight];
					tile.setPosition(sf::Vector2f(x * tilesize.x, y * tilesize.y) - chunk.getPosition());
					chunk.addTile(std::move(tile));
				}
			}
		}
	}
	for (auto &chunk : m_chunks)
		chunk->update();
}

bool Map::isInView(const Chunk &chunk) const
{
	auto maxX = m_gameView.getCenter().x + m_gameView.getSize().x;
	auto minX = m_gameView.getCenter().x - m_gameView.getSize().x;
	auto maxY = m_gameView.getCenter().y + m_gameView.getSize().y;
	auto minY = m_gameView.getCenter().y - m_gameView.getSize().y;
	return chunk.getPosition().x < maxX && chunk.getPosition().x + chunk.getSize().x > minX &&
		chunk.getPosition().y < maxY && chunk.getPosition().y + chunk.getSize().y > minY;
}
