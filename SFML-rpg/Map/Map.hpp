#pragma once
#include "Tileset.hpp"
#include "Layer.hpp"
#include "TileLayer.hpp"
#include <vector>
#include "TextureTile.hpp"
#include "ObjectLayer.hpp"
#include "Entity/Player.hpp"
#include "Chunk.hpp"
#include "Utility.hpp"

class Map
{
public:
	Map();
	~Map();

	Map(const Map &) = delete;
	Map(Map &) = delete;

	bool load(const std::string &filename, const sf::Vector2u &windowSize);

	void update(const sf::Time &elapsedTime);

	void addPlayer(const Player *pPlayer);

	void draw(sf::RenderTarget &target) const;

private:
	void loadTileset(const Json::Value &root);
	void loadLayer(const Json::Value &root, const std::string &layerGroup = "none");
	void loadChunks();

private:
	sf::Clock m_clock;
	sf::Time m_time;
	std::vector<std::variant<const TilebasedTileset, const ImageTileset>> m_tilesets;
	std::vector<std::variant<TileLayer, ObjectLayer>> m_layers;
	DynArray<Chunk> m_chunks;
	LayerDetails m_mapDetails;
	sf::Vector2i m_chunkSize = { 10, 10 };
	// Sprites have the size of the initial texture
	// Sprites are small and efficient objects
	sf::Sprite m_mapSprite, m_minimapSprite;
	sf::Font m_font;
	// Textures in general are big and inefficient objects since they store images
	sf::RenderTexture m_texture;
	sf::View m_gameView, m_minimapView;
	const Player *m_player = nullptr;
};