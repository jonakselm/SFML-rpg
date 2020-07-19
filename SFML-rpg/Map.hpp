#pragma once
#include "Tileset.hpp"

class Map
{
public:
	Map();
	~Map();

	bool load(const std::string &filename);

	void draw(sf::RenderTarget &target);

private:
	void loadTileset(const Json::Value &root);

private:
	Tileset m_tileset;
	sf::Vector2i m_size, m_tileSize;
	sf::Sprite m_shape;
};