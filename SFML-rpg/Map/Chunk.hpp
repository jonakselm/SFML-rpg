#pragma once
#include "TextureTile.hpp"

class Chunk
{
public:
	Chunk(sf::Vector2i chunkGridsize, sf::Vector2i tilesize, sf::Vector2f gridpos);

	void addTile(TextureTile &&tile);
	std::vector<TextureTile> &getTiles();

	void draw(sf::RenderTarget &target, sf::RenderStates states = sf::RenderStates::Default) const;
	void drawToChunk();

	void setAnimated(bool isAnimated);
	bool hasAnimation() const;

	void setGridSize(sf::Vector2i size);
	sf::Vector2i getGridSize() const;
	sf::Vector2i getSize() const;
	sf::Vector2i getBottomRight() const;
	const sf::Vector2f &getPosition() const;

private:
	sf::RenderTexture m_texture;
	sf::Sprite m_sprite;
	std::vector<TextureTile> m_tiles;
	bool m_hasAnimation = false;
	sf::Vector2i m_gridSize;
};
