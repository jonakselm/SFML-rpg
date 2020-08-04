#pragma once
#include "Tileset.hpp"

class TextureTile
{
public:
	TextureTile();
	~TextureTile();

	// Updates a tile if it is an animated tile
	int update(const sf::Time &elapsedTime);

	// Returns true if the tileId is higher than 0 (which means nothing is in the tile)
	bool load(int gid, const std::shared_ptr<const Tileset> &tileset, const sf::Vector2i &gridPos);

	void draw(sf::RenderTarget &target) const;

private:
	sf::IntRect getTextureRect(int localTileId, const Tileset &tileset, const sf::Vector2i &tileSize) const;

private:
	static constexpr int m_edges = 4;
	sf::Vertex m_vertices[m_edges];
	std::shared_ptr<const Tileset> m_tileset;
	std::shared_ptr<const sf::Texture> m_texture;
	bool m_animated = false;
	int m_gid = 0;
	AnimatedTile m_aniTile;
	int m_aniCount = 0;
	sf::Time m_timer;
};