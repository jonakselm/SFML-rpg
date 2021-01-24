#pragma once
#include "TextureTile.hpp"
#include "TileLayer.hpp"

class Chunk
{
public:
	Chunk() = default;
	bool addTile(int gid, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets, const sf::Vector2i &gridPos, float opacity);
	void addTiles(const TileLayer &layer, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets);

	void update(const sf::Time &elapsedTime);
	// Use this after setting properties
	// Only like this cause im lazy
	void load();

	// Remember to set properties before gridPos
	void setGridPos(const sf::Vector2i &gridPos);
	void setGridPos(int gridX, int gridY);

	void setProperties(const LayerDetails &props);
	void setProperties(const sf::Vector2i &tileSize, const sf::Vector2i &gridSize);
	void setProperties(const sf::Vector2i &tileSize, int width, int height);
	void setProperties(int tileWidth, int tileHeight, const sf::Vector2i &gridSize);
	void setProperties(int tileWidth, int tileHeight, int gridWidth, int gridHeight);

	void drawToTarget(sf::RenderTarget &target) const;
	void drawToTexture();

private:
	std::vector<TextureTile> m_tiles;
	sf::RenderTexture m_renderTexture;
	sf::Sprite m_sprite;
	LayerDetails m_chunkProps;
	sf::Vector2i m_gridPos;
	bool containsAnimations = false;
};