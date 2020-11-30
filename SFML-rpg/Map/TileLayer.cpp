#include "stdafx.h"
#include "TileLayer.hpp"
#include <iostream>

void TileLayer::update(const sf::Time &elapsedTime)
{
	for (auto &pTile : m_tiles)
		pTile->updateTile(elapsedTime);
}

void TileLayer::load(const Json::Value root, const std::string &layerGroup, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets, const sf::Vector2i &mapSize)
{
	Layer::load(root, layerGroup);
	data.reserve(root["data"].size());
	for (unsigned int i = 0; i < root["data"].size(); i++)
	{
		data.push_back(root["data"][i].asInt());
	}

	int count = 0;

	for (int y = 0; y < mapSize.y; y++)
	{
		for (int x = 0; x < mapSize.x; x++)
		{
			int gid = data[x + y * mapSize.x];

			const GenericTileset *genericTileset = tilesets[getTilesetIndex(gid, tilesets)].get();
			const TilebasedTileset &tileset = *(dynamic_cast<const TilebasedTileset*>(genericTileset));

			std::unique_ptr<TextureTile> pTextureTile;
			if (pTextureTile->load(gid, tileset, sf::Vector2i(x, y)))
				m_tiles.push_back(std::move(pTextureTile));
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
	for (const auto &pTile : m_tiles)
		pTile->draw(target);
}