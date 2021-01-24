#pragma once
#include "Layer.hpp"
#include "TextureTile.hpp"

class ObjectTile
{
public:

	TextureTile tile;
	std::variant<Object, TemplateObject> object;
};

class ObjectLayer : public Layer
{
public:
	ObjectLayer() = default;
	~ObjectLayer() = default;

	void update(const sf::Time &elapsedTime);

	void load(const Json::Value root, const std::string &layerGroup, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets,
		const LayerDetails &mapDetails);

	void draw(sf::RenderTarget &target) const;

protected:
	std::vector<ObjectTile> m_objects;
};