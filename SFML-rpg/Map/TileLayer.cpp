#include "stdafx.h"
#include "TileLayer.hpp"
#include <iostream>

void TileLayer::update(const sf::Time &elapsedTime)
{
	for (auto &tile : m_tiles)
		tile.update(elapsedTime);
}

void TileLayer::load(const Json::Value root, const std::vector<Tileset> &tilesets, const sf::Vector2i &mapSize)
{
	Layer::load(root);

	int count = 0;

	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			int gid = data[x + y * mapSize.x];

			const Tileset &tileset = tilesets[getTilesetIndex(gid, tilesets)];
			TextureTile textureTile;
			if (textureTile.load(gid, tileset, sf::Vector2i(x, y)))
				m_tiles.push_back(std::move(textureTile));
			else
				if (count < 20)
				{
					std::cout << "No tile created" << std::endl;
					count++;
				}
				else if (count == 20)
				{
					std::cout << "The message limit for this layer has been reached" << std::endl;
					count++;
				}
		}
	}
}

void TileLayer::draw(sf::RenderTarget &target) const
{
	for (const auto &tile : m_tiles)
		tile.draw(target);
}

unsigned int TileLayer::getTilesetIndex(int gid, const std::vector<Tileset> &tilesets) const
{
	int tilesetIndex = 0;
	for (const auto &tileset : tilesets)
	{
		if (!(gid > tileset.firstgid + tileset.tileCount - 1))
		{
			return tilesetIndex;
		}
		tilesetIndex++;
	}
	return 0;
}
