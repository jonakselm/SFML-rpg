#include "stdafx.h"
#include "Layer.hpp"

Layer::Layer()
{
}

void Layer::loadTiles(const Json::Value &root, const std::string &group)
{
	for (unsigned int i = 0; i < root["data"].size(); i++)
		m_tiles.push_back(root["data"][i].asInt());
	m_size = sf::Vector2i(root["width"].asInt(), root["height"].asInt());
	m_group = group;
}

int Layer::getTile(const sf::Vector2i &gridPos) const
{
	return m_tiles[gridPos.x + m_size.x * gridPos.y];
}

const std::vector<int> Layer::getTiles() const
{
	return m_tiles;
}

const sf::Vector2i &Layer::getSize() const
{
	return m_size;
}

const std::string &Layer::getGroup() const
{
	return m_group;
}
