#include "stdafx.h"
#include "Map.hpp"

Map::Map()
{
}

Map::~Map()
{
}

bool Map::load(const std::string &filename)
{
	Json::Value root;

	std::ifstream file(filename);

	file >> root;

	m_size.x = root["width"].asInt();
	m_size.y = root["height"].asInt();

	m_tileSize.x = root["tilewidth"].asInt();
	m_tileSize.y = root["tileheight"].asInt();

	loadTileset(root);

	return true;
}

void Map::draw(sf::RenderTarget &target)
{
	target.draw(m_shape);
}

void Map::loadTileset(const Json::Value &root)
{
	//Json::Value val;
	auto val = root["tilesets"].begin();

	m_tileset.load((*val));
	
	m_shape.setTexture(m_tileset.texture);
}
