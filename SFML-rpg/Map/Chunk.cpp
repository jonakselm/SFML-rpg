#include "stdafx.h"
#include "Chunk.hpp"

bool Chunk::addTile(int gid, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets, const sf::Vector2i &gridPos, float opacity)
{
	const std::variant<const TilebasedTileset, const ImageTileset> &tileset = tilesets[getTilesetIndex(gid, tilesets)];

	TextureTile textureTile;
	if (textureTile.load(gid, tileset, gridPos, opacity))
	{
		m_tiles.push_back(textureTile);
		if (textureTile.isAnimated())
			containsAnimations = true;
		return true;
	}
	return false;
}

void Chunk::addTiles(const TileLayer &layer, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets)
{
	// Add tiles with the chunk's gridPos as offset, but keep the
	// tile's gridPos from 0 (since we're using renderTexture)
	for (int y = 0; y < m_chunkProps.height; y++)
	{
		for (int x = 0; x < m_chunkProps.width; x++)
		{
			sf::Vector2i layerGridPos = { x + m_gridPos.x, y + m_gridPos.y };
			if (layerGridPos.x < layer.properties.layerDetails.width &&
				layerGridPos.y < layer.properties.layerDetails.height)
				addTile(layer.getData(layerGridPos), tilesets, sf::Vector2i(x, y), layer.properties.opacity);
		}
	}
}

void Chunk::update(const sf::Time &elapsedTime)
{
	if (containsAnimations)
	{
		m_renderTexture.clear();
		for (auto &tile : m_tiles)
		{
			tile.updateTile(elapsedTime);
			tile.draw(m_renderTexture);
		}
		m_renderTexture.display();
	}
}

void Chunk::load()
{
	m_renderTexture.create(m_chunkProps.tileSize.x * m_chunkProps.width,
		m_chunkProps.tileSize.y * m_chunkProps.height);
	m_sprite.setTexture(m_renderTexture.getTexture());
}

void Chunk::setGridPos(const sf::Vector2i &gridPos)
{
	m_gridPos = gridPos;
	m_sprite.setPosition(float(gridPos.x * m_chunkProps.tileSize.x), float(gridPos.y * m_chunkProps.tileSize.y));
}

void Chunk::setGridPos(int gridX, int gridY)
{
	setGridPos(sf::Vector2i(gridX, gridY));
}

void Chunk::setProperties(const LayerDetails &props)
{
	m_chunkProps = props;
}

void Chunk::setProperties(const sf::Vector2i &tileSize, const sf::Vector2i &gridSize)
{
	setProperties({ tileSize, gridSize.x, gridSize.y });
}

void Chunk::setProperties(const sf::Vector2i &tileSize, int gridWidth, int gridHeight)
{
	setProperties({ tileSize, gridWidth, gridHeight });
}

void Chunk::setProperties(int tileWidth, int tileHeight, const sf::Vector2i &gridSize)
{
	setProperties({ { tileWidth, tileHeight }, gridSize.x, gridSize.y });
}

void Chunk::setProperties(int tileWidth, int tileHeight, int gridWidth, int gridHeight)
{
	setProperties({ { tileWidth, tileHeight }, gridWidth, gridHeight });
}

void Chunk::drawToTarget(sf::RenderTarget &target) const
{
	target.draw(m_sprite);
}

void Chunk::drawToTexture()
{
	m_renderTexture.clear();
	for (const auto &tile : m_tiles)
	{
		tile.draw(m_renderTexture);
	}
	m_renderTexture.display();
}
