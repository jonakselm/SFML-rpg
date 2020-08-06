#include "stdafx.h"
#include "TextureTile.hpp"

TextureTile::TextureTile()
{
}

TextureTile::~TextureTile()
{
	m_texture = nullptr;
}

int TextureTile::update(const sf::Time &elapsedTime)
{
	m_timer += elapsedTime;
	if (m_animated)
	{
		if (m_timer.asMilliseconds() > m_aniTile.animation[m_aniCount].duration)
		{
			m_aniCount++;
			if (m_aniCount >= m_aniTile.animation.size())
				m_aniCount = 0;

			m_timer = sf::Time::Zero;
		}

		int localTileId = m_aniTile.animation[m_aniCount].tileId;

		sf::IntRect textureRect = getTextureRect(localTileId, (*m_tileset), m_tileset->tileSize);
		int textureRight = textureRect.left + textureRect.width,
			textureBottom = textureRect.top + textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
		m_vertices[1].texCoords = sf::Vector2f(textureRight, textureRect.top);
		m_vertices[2].texCoords = sf::Vector2f(textureRight, textureBottom);
		m_vertices[3].texCoords = sf::Vector2f(textureRect.left, textureBottom);

		return m_aniCount + 1;
	}
	return 0;
}

bool TextureTile::load(int gid, const Tileset &tileset, const sf::Vector2i &gridPos)
{
	if (gid <= 0)
		return false;

	m_tileset = &tileset;
	m_texture = &tileset.texture;

	int localTileId = gid - tileset.firstgid;

	int left = gridPos.x * tileset.tileSize.x, top = gridPos.y * tileset.tileSize.x,
		right = (gridPos.x + 1) * tileset.tileSize.x, bottom = (gridPos.y + 1) * tileset.tileSize.y;

	m_vertices[0].position = sf::Vector2f(left, top);
	m_vertices[1].position = sf::Vector2f(right, top);
	m_vertices[2].position = sf::Vector2f(right, bottom);
	m_vertices[3].position = sf::Vector2f(left, bottom);

	for (const auto &aniTile : tileset.animatedTiles)
	{
		if (localTileId == aniTile.id)
		{
			m_animated = true;
			m_aniTile = aniTile;

			localTileId = m_aniTile.animation[m_aniCount].tileId;
			break;
		}
	}

	sf::IntRect textureRect = getTextureRect(localTileId, tileset, tileset.tileSize);
	int textureRight = textureRect.left + textureRect.width,
		textureBottom = textureRect.top + textureRect.height;

	m_vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
	m_vertices[1].texCoords = sf::Vector2f(textureRight, textureRect.top);
	m_vertices[2].texCoords = sf::Vector2f(textureRight, textureBottom);
	m_vertices[3].texCoords = sf::Vector2f(textureRect.left, textureBottom);

	return true;
}

void TextureTile::draw(sf::RenderTarget &target) const
{
	target.draw(m_vertices, EDGE_COUNT, sf::Quads, m_texture);
}

sf::IntRect TextureTile::getTextureRect(int localTileId, const Tileset &tileset, const sf::Vector2i &tileSize) const
{
	int x = localTileId % tileset.columns, y = localTileId / tileset.columns;

	return sf::IntRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y);
}
