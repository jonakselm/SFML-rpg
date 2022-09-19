#include "stdafx.h"
#include "Tileset.hpp"

void Tileset::load(Json::Value root)
{
	m_name = root["name"].asString();
	m_columns = root["columns"].asInt();
	m_tilesize = sf::Vector2i(root["tilewidth"].asInt(), root["tileheight"].asInt());
	m_gridsize = sf::Vector2i(root["imagewidth"].asInt() / m_tilesize.x, 
		root["imageheight"].asInt() / m_tilesize.y);
	std::string source = "data/maps/" + root["image"].asString();
	if (!m_texture.loadFromFile(source))
	{
		exit(100);
	}
}

void Tileset::setFirstgid(int firstgid)
{
	m_firstgid = firstgid;
}

int Tileset::getFirstgid() const
{
	return m_firstgid;
}

const sf::Texture &Tileset::getTexture() const
{
	return m_texture;
}

const sf::Vector2i &Tileset::getSize() const
{
	return m_gridsize;
}

const sf::Vector2i &Tileset::getTilesize() const
{
	return m_tilesize;
}
