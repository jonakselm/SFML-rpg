#pragma once
#include "Layer.hpp"
#include "TextureTile.hpp"

class TileLayer : public Layer
{
public:
	TileLayer() = default;
	~TileLayer() = default;

	void update(const sf::Time &elapsedTime);

	void load(const Json::Value root, const std::string &layerGroup, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets,
		const sf::Vector2i &mapSize);

	void draw(sf::RenderTarget &target) const;

private:
	std::vector<int> data;
	std::vector<std::unique_ptr<TextureTile>> m_tiles;
};