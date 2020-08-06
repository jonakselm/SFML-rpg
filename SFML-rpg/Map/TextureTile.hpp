#pragma once
#include "Tileset.hpp"

class TextureTile
{
public:
	TextureTile();
	~TextureTile();

	// Updates a tile if it is an animated tile
	bool update(const sf::Time &elapsedTime);

	// Returns true if the tileId is higher than 0 (which means nothing is in the tile)
	bool load(int gid, const Tileset &tileset, const sf::Vector2i &gridPos);

	void draw(sf::RenderTarget &target) const;

private:
	sf::IntRect getTextureRect(int localTileId, const Tileset &tileset, const sf::Vector2i &tileSize) const;

private:
	static constexpr int EDGE_COUNT = 4;
	sf::Vertex m_vertices[EDGE_COUNT];
	const Tileset *m_tileset;
	const sf::Texture *m_texture;
	bool m_animated = false;
	AnimatedTile m_aniTile;
	int m_aniCount = 0;
	sf::Time m_timer;
	int m_localTileId = 0;
};