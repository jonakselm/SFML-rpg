#pragma once
#include "Layer.hpp"
#include "TextureTile.hpp"

class TileLayer : public Layer
{
public:
	TileLayer() = default;
	~TileLayer() = default;

	void update(const sf::Time &elapsedTime);

	void load(const Json::Value root, const std::vector<Tileset> &tilesets,
		const sf::Vector2i &mapSize);

	void draw(sf::RenderTarget &target) const;

private:
	// Returns 0 if an invalid gid value is entered
	unsigned int getTilesetIndex(int gid, const std::vector<Tileset> &tilesets) const;

private:
	std::vector<TextureTile> m_tiles;
};