#include "stdafx.h"
#include "Chunk.hpp"

Chunk::Chunk(sf::Vector2i chunkGridsize, sf::Vector2i tilesize, sf::Vector2f gridpos)
	:
	m_gridSize(chunkGridsize)
{
	m_texture.create(chunkGridsize.x * tilesize.x, chunkGridsize.y * tilesize.y);
	m_sprite.setTexture(m_texture.getTexture());
	sf::Vector2f pos = sf::Vector2f(chunkGridsize.x * tilesize.x * gridpos.x,
		chunkGridsize.y * tilesize.y * gridpos.y);
	m_sprite.setPosition(pos);
}

void Chunk::addTile(TextureTile &&tile)
{
	m_tiles.push_back(tile);
}

std::vector<TextureTile> &Chunk::getTiles() 
{
	return m_tiles;
}

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Chunk::drawToChunk()
{
	m_texture.clear(sf::Color::Transparent);
	for (const auto &tile : m_tiles)
	{
		m_texture.draw(tile);
	}
	m_texture.display();
}

void Chunk::setAnimated(bool isAnimated)
{
	m_hasAnimation = isAnimated;
}

bool Chunk::hasAnimation() const
{
	return m_hasAnimation;
}

void Chunk::setGridSize(sf::Vector2i size)
{
	m_gridSize = size;
}

sf::Vector2i Chunk::getGridSize() const
{
	return m_gridSize;
}

sf::Vector2i Chunk::getSize() const
{
	return static_cast<sf::Vector2i>(m_texture.getSize());
}

sf::Vector2i Chunk::getBottomRight() const
{
	return sf::Vector2i(m_sprite.getPosition() + sf::Vector2f(m_texture.getSize()));
}

const sf::Vector2f &Chunk::getPosition() const
{
	return m_sprite.getPosition();
}
