#pragma once
#include "Tileset.hpp"
#include "Layer.hpp"
#include "TileLayer.hpp"
#include <vector>
#include "TextureTile.hpp"
#include "ObjectLayer.hpp"

class Map
{
public:
	Map();
	~Map();

	Map(const Map &) = delete;
	Map(Map &) = delete;

	bool load(const std::string &filename, const sf::Vector2u &windowSize);

	void update(const sf::Time &elapsedTime);

	void draw(sf::RenderTarget &target) const;

private:
	void loadTileset(const Json::Value &root);
	void loadLayer(const Json::Value &root, const std::string &layerGroup = "none");

private:
	sf::Clock m_clock;
	sf::Time m_time;
	std::vector<std::unique_ptr<const GenericTileset>> m_tilesets;
	std::vector<std::unique_ptr<Layer>> m_layers;
	sf::Vector2i m_mapSize, m_tileSize;
	// Sprites have the size of the initial texture
	sf::Sprite m_sprite;
	sf::Font m_font;
	sf::RenderTexture m_texture;
	sf::View m_gameView, m_minimapView;
};