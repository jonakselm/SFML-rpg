#pragma once
#include "Layer.hpp"
#include "TextureTile.hpp"

class TileLayer : public Layer
{
public:
	TileLayer() = default;
	~TileLayer() = default;

	void update(const sf::Time &elapsedTime);

	void load(const Json::Value root, const std::string &layerGroup, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets,
		const LayerDetails &mapDetails);

	void draw(sf::RenderTarget &target) const;

private:
	std::vector<int> data;
	std::vector<TextureTile> m_tiles;
};