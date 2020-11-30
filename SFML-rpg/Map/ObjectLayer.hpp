#pragma once
#include "Layer.hpp"
#include "TextureTile.hpp"

class Object
{
public:
	Object() = default;
	~Object() = default;

	int id = 0;
	TileTemplate tileTemplate;
	sf::Vector2f pos;
	TextureTile tile;
};

class ObjectLayer : public Layer
{
public:
	ObjectLayer() = default;
	~ObjectLayer() = default;

	void load(const Json::Value root, const std::string &layerGroup, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets,
		const sf::Vector2i &mapSize);

	void update(const sf::Time &elapsedTime);

	void draw(sf::RenderTarget &target) const;

private:
	std::vector<Object> m_objects;
	std::vector<std::unique_ptr<Object>> m_objects;
};