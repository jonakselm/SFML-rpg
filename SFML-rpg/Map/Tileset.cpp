#include "stdafx.h"
#include "Tileset.hpp"

void Tileset::load(Json::Value root)
{
	m_name = root["name"].asString();
	m_columns = root["columns"].asInt();
	m_tilesize = sf::Vector2i(root["tilewidth"].asInt(), root["tileheight"].asInt());
	m_gridsize = sf::Vector2i(root["imagewidth"].asInt() / m_tilesize.x, 
		root["imageheight"].asInt() / m_tilesize.y);
	std::string source = "../SFML-rpg/data/maps/" + root["image"].asString();
	if (!m_texture.loadFromFile(source))
	{
		exit(100);
	}
	for (int i = 0; i < root["tiles"].size(); i++)
	{
		Json::Value val = root["tiles"][i];
		int localId = val["id"].asInt();
		if (val["animations"].size())
			m_animations.try_emplace(localId);
		for (int i = 0; i < val["animation"].size(); i++)
		{
			Json::Value anim = val["animation"][i];
			auto &frame = m_animations[localId].emplace_back();
			frame.dur = sf::milliseconds(anim["duration"].asInt());
			frame.localId = anim["tileid"].asInt();
			frame.textureRect = getTextureRect(frame.localId);
		}
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

const std::vector<Frame> * const Tileset::getAnimationFrames(int localId) const
{
	if (m_animations.contains(localId))
	{
		return &m_animations.at(localId);
	}
	return nullptr;
}

sf::IntRect Tileset::getTextureRect(int localId) const
{
	auto topLeft = sf::Vector2i(localId % m_gridsize.x * m_tilesize.x, localId / m_gridsize.x * m_tilesize.y);
	return sf::IntRect(topLeft, m_tilesize);
}
