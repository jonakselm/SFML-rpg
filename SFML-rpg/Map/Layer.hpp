#pragma once
#include "Tileset.hpp"

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void load(const Json::Value &root, const std::string &group);

	virtual void draw(sf::RenderTarget &target) const = 0;

	virtual void update(const sf::Time &time) = 0;

	int id;
	std::string type;
	std::string name;
	std::string group;
	sf::Vector2i size;
	float opacity;
	bool visible;

protected:
	// Returns 0 if an invalid gid value is entered
	unsigned int getTilesetIndex(int gid, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets) const;
};