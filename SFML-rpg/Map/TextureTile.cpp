#include "stdafx.h"
#include "TextureTile.hpp"

TextureTile::TextureTile()
{
}

TextureTile::~TextureTile()
{
	m_tileset = nullptr;
}

bool TextureTile::updateTile(const sf::Time &elapsedTime)
{
	if (m_animated)
	{
		m_timer += elapsedTime;
		if (m_timer.asMilliseconds() > m_animation->frames[m_animationCount].duration)
		{
			m_animationCount++;
			if (m_animationCount >= m_animation->frames.size())
				m_animationCount = 0;

			m_timer = sf::Time::Zero;
		}

		int localTileId = m_animation->frames[m_animationCount].tileId;

		const TilebasedTileset *pTileset = nullptr;
		if (std::holds_alternative<const TilebasedTileset>(*m_tileset))
			pTileset = &std::get<const TilebasedTileset>(*m_tileset);

		sf::IntRect textureRect = getTextureRect(localTileId, *pTileset, pTileset->tileSize);
		int textureRight = textureRect.left + textureRect.width,
			textureBottom = textureRect.top + textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(float(textureRect.left), float(textureRect.top));
		m_vertices[1].texCoords = sf::Vector2f(float(textureRight), float(textureRect.top));
		m_vertices[2].texCoords = sf::Vector2f(float(textureRight), float(textureBottom));
		m_vertices[3].texCoords = sf::Vector2f(float(textureRect.left), float(textureBottom));

		return true;
	}
	return false;
}

bool TextureTile::updateObject(const sf::Time &elapsedTime)
{
	if (m_animated)
	{
		m_timer += elapsedTime;
		if (m_timer.asMilliseconds() > m_animation->frames[m_animationCount].duration)
		{
			m_animationCount++;
			if (m_animationCount >= m_animation->frames.size())
				m_animationCount = 0;

			m_timer = sf::Time::Zero;
		}

		int localTileId = m_animation->frames[m_animationCount].tileId;

		const ImageTileset *pTileset = nullptr;
		if (std::holds_alternative<const ImageTileset>(*m_tileset))
			pTileset = &std::get<const ImageTileset>(*m_tileset);

		const ImageTile &imageTile = pTileset->imageTiles[localTileId];

		m_renderStates.texture = &imageTile.texture;

		sf::IntRect textureRect = getTextureRect(localTileId, *pTileset, imageTile.imageSize);
		int textureRight = textureRect.left + textureRect.width,
			textureBottom = textureRect.top + textureRect.height;

		m_vertices[0].texCoords = sf::Vector2f(float(textureRect.left), float(textureRect.top));
		m_vertices[1].texCoords = sf::Vector2f(float(textureRight), float(textureRect.top));
		m_vertices[2].texCoords = sf::Vector2f(float(textureRight), float(textureBottom));
		m_vertices[3].texCoords = sf::Vector2f(float(textureRect.left), float(textureBottom));

		return true;
	}
	return false;
}

bool TextureTile::loadWithMapCoords(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::FloatRect &worldRect, float opacity)
{
	const GenericTileset *genericTileset = nullptr;
	if (std::holds_alternative<const TilebasedTileset>(tileset))
		genericTileset = &std::get<const TilebasedTileset>(tileset);
	else
		genericTileset = &std::get<const ImageTileset>(tileset);

	m_localTileId = gid - genericTileset->firstgid;

	if (dynamic_cast<const TilebasedTileset *>(genericTileset))
	{
		const TilebasedTileset *pTileset = dynamic_cast<const TilebasedTileset *>(genericTileset);
		m_renderStates.texture = &pTileset->texture;
	}
	else if (dynamic_cast<const ImageTileset *>(genericTileset))
	{
		const ImageTileset *pTileset = dynamic_cast<const ImageTileset *>(genericTileset);
		m_renderStates.texture = &pTileset->imageTiles[m_localTileId].texture;
	}

	return genericLoad(gid, tileset, worldRect, opacity);
}

bool TextureTile::load(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::Vector2i &gridPos, float opacity)
{
	const GenericTileset *genericTileset = nullptr;
	if (std::holds_alternative<const TilebasedTileset>(tileset))
		genericTileset = &std::get<const TilebasedTileset>(tileset);
	else
		genericTileset = &std::get<const ImageTileset>(tileset);

	int left = gridPos.x * int(genericTileset->tileSize.x),
		bottom = (gridPos.y + 1) * int(genericTileset->tileSize.x);

	// Tiled uses bottom-left (not top-left)
	const sf::FloatRect worldRect = { static_cast<float>(left), static_cast<float>(bottom),
		genericTileset->tileSize.x, genericTileset->tileSize.y };

	return loadWithMapCoords(gid, tileset, worldRect, opacity);
}

bool TextureTile::load(const Object &object, float opacity)
{
	m_transformable.setRotation(object.objProps.rotation);
	// Need to make this in a way that it takes both image tilesets and tilebased tilesets
	return loadWithMapCoords(object.gid, *object.pTileset, object.objProps.rect, opacity);
}

bool TextureTile::load(const TemplateObject &templateObject, float opacity)
{
	const int gid = templateObject.gid;

	if (templateObject.tilebased)
	{
		const TilebasedTileset &tileset = std::get<const TilebasedTileset>(*templateObject.pTileset);
		m_renderStates.texture = &tileset.texture;
		m_localTileId = gid - tileset.firstgid;
	}
	else
	{
		// Tiled starts indexing from 1 instead of 0, correcting that
		m_localTileId = templateObject.gid - 1;

		const ImageTileset &tileset = std::get<const ImageTileset>(*templateObject.pTileset);

		if (gid >= tileset.firstgid && gid <= tileset.firstgid + tileset.tileCount)
		{
			m_localTileId -= tileset.firstgid;

			m_renderStates.texture = &tileset.imageTiles[m_localTileId].texture;
		}
		else if (gid < tileset.firstgid)
		{
			m_renderStates.texture = &tileset.imageTiles[m_localTileId].texture;
		}
	}
	m_transformable.setRotation(templateObject.objProps.rotation);

	return genericLoad(gid, *templateObject.pTileset, templateObject.objProps.rect, opacity);
}

const sf::Vector2f &TextureTile::getPosition() const
{
	return m_transformable.getPosition();
}

const sf::Vector2f &TextureTile::getSize() const
{
	// Bottom-right side
	return m_vertices[2].position;
}

const sf::FloatRect &TextureTile::getRect() const
{
	return sf::FloatRect(getPosition(), getSize());
}

void TextureTile::draw(sf::RenderTarget &target) const
{
	target.draw(m_vertices, EDGE_COUNT, sf::Quads, m_renderStates);
}

bool TextureTile::genericLoad(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::FloatRect &rect, float opacity)
{
	if (gid <= 0)
		return false;

	for (int i = 0; i < 4; i++)
		m_vertices[i].color = sf::Color(255, 255, 255, sf::Uint8((256 * opacity) - 1));
	int i = 0;

	m_tileset = &tileset;

	const GenericTileset *genericTileset = nullptr;
	if (std::holds_alternative<const TilebasedTileset>(tileset))
		genericTileset = &std::get<const TilebasedTileset>(tileset);
	else
		genericTileset = &std::get<const ImageTileset>(tileset);

	// Tiled starts from bottom left (setting the origin to bottom left)
	m_transformable.setOrigin(0, rect.height);

	m_vertices[0].position = sf::Vector2f(0, 0);
	m_vertices[1].position = sf::Vector2f(rect.width, 0);
	m_vertices[2].position = sf::Vector2f(rect.width, rect.height);
	m_vertices[3].position = sf::Vector2f(0, rect.height);

	m_transformable.setPosition(rect.left, rect.top);

	sf::Vector2f tileSize;

	if (dynamic_cast<const TilebasedTileset *>(genericTileset))
	{
		const TilebasedTileset *pTileset = dynamic_cast<const TilebasedTileset *>(genericTileset);
		tileSize = pTileset->tileSize;
	}
	else if (dynamic_cast<const ImageTileset *>(genericTileset))
	{
		const ImageTileset *pTileset = dynamic_cast<const ImageTileset *>(genericTileset);
		tileSize = pTileset->imageTiles[m_localTileId].imageSize;
	}

	sf::IntRect textureRect = getTextureRect(m_localTileId, *genericTileset, tileSize);
	int textureRight = textureRect.left + textureRect.width,
		textureBottom = textureRect.top + textureRect.height;

	m_vertices[0].texCoords = sf::Vector2f(float(textureRect.left), float(textureRect.top));
	m_vertices[1].texCoords = sf::Vector2f(float(textureRight),	float(textureRect.top));
	m_vertices[2].texCoords = sf::Vector2f(float(textureRight),	float(textureBottom));
	m_vertices[3].texCoords = sf::Vector2f(float(textureRect.left), float(textureBottom));

	for (const auto &tileProps : genericTileset->tileProperties)
	{
		if (tileProps.id == m_localTileId)
		{
			if (tileProps.animation.frames.size() >= 1)
			{
				m_animated = true;
				m_localTileId = tileProps.animation.frames[m_animationCount].tileId;
			}
			m_animation = &tileProps.animation;
			break;
		}
	}

	m_renderStates.transform = m_transformable.getTransform();

	return true;
}

sf::IntRect TextureTile::getTextureRect(int localTileId, const GenericTileset &tileset, const sf::Vector2f &tileSize) const
{
	int x = 0, y = 0;
	if (dynamic_cast<const TilebasedTileset*>(&tileset))
	{
		const TilebasedTileset &tTileset = *(dynamic_cast<const TilebasedTileset *>(&tileset));
		x = localTileId % tTileset.columns;
		y = localTileId / tTileset.columns;
	}

	return sf::IntRect(int(x * tileSize.x), int(y * tileSize.y), int(tileSize.x), int(tileSize.y));
}
