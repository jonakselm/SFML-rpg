#pragma once
#include "Tileset.hpp"

class TextureTile
{
public:
	TextureTile();
	~TextureTile();

	// Messing with std::variant
	/*TextureTile(const TextureTile &) = delete;
	TextureTile(TextureTile &) = delete;
	TextureTile &operator=(const TextureTile &) = delete;
	TextureTile &operator=(TextureTile &) = delete;*/

	// Updates a tile if it is an animated tile
	bool updateTile(const sf::Time &elapsedTime);
	bool updateObject(const sf::Time &elapsedTime);

	// Returns true if the tileId is higher than 0 (which means nothing is in the tile)
	// Remember tiled uses bottom-left (not the standard top-left)
	bool loadWithMapCoords(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::FloatRect &worldRect, float opacity);
	bool load(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::Vector2i &gridPos, float opacity);
	
	bool load(const Object &object, float opacity);
	bool load(const TemplateObject &templateObject, float opacity);

	const sf::Vector2f &getPosition() const;
	const sf::Vector2f &getSize() const;
	const sf::FloatRect &getRect() const;

	bool isAnimated() const;

	void draw(sf::RenderTarget &target) const;

private:
	// Uses percentage values
	bool genericLoad(int gid, const std::variant<const TilebasedTileset, const ImageTileset> &tileset, const sf::FloatRect &rect, float opacity = 1.f);
	sf::IntRect getTextureRect(int localTileId, const GenericTileset &tileset, const sf::Vector2f &tileSize) const;

private:
	static constexpr int EDGE_COUNT = 4;
	sf::Vertex m_vertices[EDGE_COUNT];
	const std::variant<const TilebasedTileset, const ImageTileset> *m_tileset = nullptr;
	sf::RenderStates m_renderStates;
	sf::Transformable m_transformable;
	const Animation *m_animation = nullptr;
	bool m_animated = false;
	int m_animationCount = 0;
	sf::Time m_timer;
	int m_localTileId = 0;
};