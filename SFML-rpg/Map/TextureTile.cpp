#include "stdafx.h"
#include "TextureTile.hpp"

TextureTile::TextureTile()
{
}

TextureTile::~TextureTile()
{
	m_pTileset = nullptr;
}

bool TextureTile::updateTile(const sf::Time &elapsedTime)
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

		const TilebasedTileset *pTileset = dynamic_cast<const TilebasedTileset *>(m_pTileset);

		sf::FloatRect textureRect = getTextureRect(localTileId, (*m_pTileset), pTileset->tileSize);
		int textureRight = textureRect.left + textureRect.width,
			textureBottom = textureRect.top + textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
		m_vertices[1].texCoords = sf::Vector2f(textureRight, textureRect.top);
		m_vertices[2].texCoords = sf::Vector2f(textureRight, textureBottom);
		m_vertices[3].texCoords = sf::Vector2f(textureRect.left, textureBottom);

		return true;
	}
	return false;
}

bool TextureTile::updateObject(const sf::Time &elapsedTime)
{
	//m_transformable.rotate(1);
	//m_states.transform = m_transformable.getTransform();
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

		const ImageTileset *pTileset = dynamic_cast<const ImageTileset *>(m_pTileset);

		const ImageTile &imageTile = pTileset->imageTiles[localTileId];

		m_states.texture = &imageTile.texture;

		sf::FloatRect textureRect = getTextureRect(localTileId, (*m_pTileset), imageTile.imageSize);
		int textureRight = textureRect.left + textureRect.width,
			textureBottom = textureRect.top + textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
		m_vertices[1].texCoords = sf::Vector2f(textureRight, textureRect.top);
		m_vertices[2].texCoords = sf::Vector2f(textureRight, textureBottom);
		m_vertices[3].texCoords = sf::Vector2f(textureRect.left, textureBottom);

		return true;
	}
	return false;
}

bool TextureTile::load(int gid, const TilebasedTileset &tileset, const sf::Vector2i &gridPos)
{
	m_localTileId = gid - tileset.firstgid;

	m_states.texture = &tileset.texture;

	// Using bottom left since tiled uses bottom left
	int left = gridPos.x * tileset.tileSize.x, bottom = (gridPos.y + 1) * tileset.tileSize.x;

	auto rect = sf::FloatRect(left, bottom, tileset.tileSize.x, tileset.tileSize.y);

	if (genericLoad(gid, tileset, rect))
		return true;

	return false;
}

bool TextureTile::load(const TileTemplate &tTemplate, const sf::FloatRect &rect, bool isTemplate)
{
	m_localTileId = tTemplate.gid;
	if (isTemplate)
		m_localTileId--;

	const int gid = tTemplate.gid;
	const ImageTileset &tileset = *tTemplate.pTileset;

	if (gid >= tileset.firstgid && gid <= tileset.firstgid + tileset.tileCount)
	{
		m_localTileId -= tileset.firstgid;

		m_states.texture = &tileset.imageTiles[m_localTileId].texture;
	}
	else if (gid < tileset.firstgid)
	{
		m_states.texture = &tileset.imageTiles[m_localTileId].texture;
	}
	m_transformable.setRotation(tTemplate.rotation);

	if (genericLoad(gid, tileset, rect))
		return true;

	return false;
}

void TextureTile::draw(sf::RenderTarget &target) const
{
	target.draw(m_vertices, EDGE_COUNT, sf::Quads, m_states);
}

bool TextureTile::genericLoad(int gid, const GenericTileset &tileset, const sf::FloatRect &rect)
{
	if (gid <= 0)
		return false;

	m_pTileset = &tileset;

	float right = rect.width, bottom = rect.height * 2;

	// Tiled starts from bottom left (setting the origin to bottom left)
	m_transformable.setOrigin(0, rect.height);

	m_vertices[0].position = sf::Vector2f(0, 0);
	m_vertices[1].position = sf::Vector2f(rect.width, 0);
	m_vertices[2].position = sf::Vector2f(rect.width, rect.height);
	m_vertices[3].position = sf::Vector2f(0, rect.height);

	m_transformable.setPosition(rect.left, rect.top);

	sf::Vector2f tileSize;

	if (dynamic_cast<const TilebasedTileset *>(&tileset))
	{
		const TilebasedTileset *pTileset = dynamic_cast<const TilebasedTileset *>(&tileset);
		tileSize = pTileset->tileSize;
	}
	else if (dynamic_cast<const ImageTileset *>(&tileset))
	{
		const ImageTileset *pTileset = dynamic_cast<const ImageTileset *>(&tileset);
		tileSize = pTileset->imageTiles[m_localTileId].imageSize;
	}

	sf::FloatRect textureRect = getTextureRect(m_localTileId, tileset, tileSize);
	int textureRight = textureRect.left + textureRect.width,
		textureBottom = textureRect.top + textureRect.height;

	m_vertices[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
	m_vertices[1].texCoords = sf::Vector2f(textureRight, textureRect.top);
	m_vertices[2].texCoords = sf::Vector2f(textureRight, textureBottom);
	m_vertices[3].texCoords = sf::Vector2f(textureRect.left, textureBottom);

	for (const auto &aniTile : tileset.animatedTiles)
	{
		if (m_localTileId == aniTile.id)
		{
			m_animated = true;
			m_aniTile = aniTile;

			m_localTileId = m_aniTile.animation[m_aniCount].tileId;
			break;
		}
	}

	m_states.transform = m_transformable.getTransform();

	return true;
}

sf::FloatRect TextureTile::getTextureRect(int localTileId, const GenericTileset &tileset, const sf::Vector2f &tileSize) const
{
	int x = 0, y = 0;
	if (dynamic_cast<const TilebasedTileset*>(&tileset))
	{
		const TilebasedTileset &tTileset = *(dynamic_cast<const TilebasedTileset *>(&tileset));
		x = localTileId % tTileset.columns;
		y = localTileId / tTileset.columns;
	}

	return sf::FloatRect(x * tileSize.x, y * tileSize.y, tileSize.x, tileSize.y);
}
