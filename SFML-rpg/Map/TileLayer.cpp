#include "stdafx.h"
#include "TileLayer.hpp"
#include <iostream>

void TileLayer::update(const sf::Time &elapsedTime)
{
	for (auto &tile : m_tiles)
		tile.updateTile(elapsedTime);
}

void TileLayer::load(const Json::Value root, const std::string &layerGroup, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets, const LayerDetails &mapDetails)
{
	Layer::load(root, layerGroup, mapDetails);
	data.reserve(root["data"].size());
	for (unsigned int i = 0; i < root["data"].size(); i++)
	{
		data.push_back(root["data"][i].asInt());
	}

	/*int count = 0;
	LayerDetails layerDetails = properties.layerDetails;

	for (int y = 0; y < layerDetails.height; y++)
	{
		for (int x = 0; x < layerDetails.width; x++)
		{
			int gid = data[size_t(x) + size_t(y) * size_t(layerDetails.width)];

			const std::variant<const TilebasedTileset, const ImageTileset> &tileset = tilesets[getTilesetIndex(gid, tilesets)];

			TextureTile textureTile;
			if (textureTile.load(gid, tileset, sf::Vector2i(x, y), properties.opacity))
			{
				m_tiles.push_back(textureTile);
			}
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
	}*/
}

int TileLayer::getData(int x, int y) const
{
	return getData(sf::Vector2i(x, y));
}

void TileLayer::draw(sf::RenderTarget &target) const
{
	for (auto &tile : m_tiles)
	{
		tile.draw(target);
	}
}

int TileLayer::getData(const sf::Vector2i &gridPos) const
{
	return data[size_t(gridPos.x) + size_t(gridPos.y) * size_t(properties.layerDetails.width)];
}