#pragma once
#include "Tileset.hpp"
#include "TextureTile.hpp"
#include "Player.hpp"
#include "Utility.hpp"
#include "Layer.hpp"
#include "Chunk.hpp"
#include "Animator.hpp"

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
	void loadTiles();
	// Only chunks within the view (with a threshold of half a view size) are rendered
	bool isInView(const Chunk &chunk) const;

private:
	sf::Clock m_clock;
	sf::Time m_time;
	Animator animator;
	// Sprites have the size of the initial texture
	// Sprites are small and efficient objects
	//sf::Sprite m_mapSprite, m_minimapSprite;
	sf::Font m_font;
	// Textures in general are big and inefficient objects since they store images
	//sf::RenderTexture m_texture;
	sf::View m_gameView, m_minimapView;
	std::vector<Tileset> m_tilesets;
	std::vector<Layer> m_layers;
	const sf::Vector2i m_chunkSize = { 20, 20 };
	std::vector<std::unique_ptr<Chunk>> m_chunks;
	const Player *m_player = nullptr;
	sf::Vector2i m_mapsize, m_tilesize;
	sf::Shader m_opacityShader;
};
