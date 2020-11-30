#pragma once
#include "Tileset.hpp"

class TextureTile
{
public:
	TextureTile();
	~TextureTile();

	TextureTile(const TextureTile &) = delete;
	TextureTile(TextureTile &) = delete;
	TextureTile &operator=(const TextureTile &) = delete;
	TextureTile &operator=(TextureTile &) = delete;

	// Updates a tile if it is an animated tile
	bool updateTile(const sf::Time &elapsedTime);
	bool updateObject(const sf::Time &elapsedTime);

	// Returns true if the tileId is higher than 0 (which means nothing is in the tile)
	bool load(int gid, const TilebasedTileset &tileset, const sf::Vector2i &gridPos);
	// Returns true if the tileId is higher than 0 (which means nothing is in the tile)
	bool load(const TileTemplate &tTemplate, const sf::FloatRect &rect, bool isTemplate);

	void draw(sf::RenderTarget &target) const;

private:
	bool genericLoad(int gid, const GenericTileset &tileset, const sf::FloatRect &rect);
	sf::FloatRect getTextureRect(int localTileId, const GenericTileset &tileset, const sf::Vector2f &tileSize) const;

private:
	static constexpr int EDGE_COUNT = 4;
	sf::Vertex m_vertices[EDGE_COUNT];
	const GenericTileset *m_pTileset;
	sf::RenderStates m_states;
	sf::Transformable m_transformable;
	bool m_animated = false;
	AnimatedTile m_aniTile;
	int m_aniCount = 0;
	sf::Time m_timer;
	int m_localTileId = 0;
};