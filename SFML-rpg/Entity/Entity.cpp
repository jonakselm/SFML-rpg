#include "stdafx.h"
#include "Entity.hpp"

bool Entity::load(const std::string &filename, const sf::Vector2f &tileSize)
{
	if (!m_texture.loadFromFile(filename))
		return false;
	m_sprite.setTexture(m_texture);

	m_size = tileSize;

	return true;
}
