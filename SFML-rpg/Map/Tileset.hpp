#pragma once
#include "Utility.hpp"
#include <unordered_map>
#include <filesystem>

class Tileset
{
public:
	void load(const std::filesystem::path& sourcePath, Json::Value root);

	void setFirstgid(int firstgid);
	int getFirstgid() const;

	const sf::Texture &getTexture() const;
	// Returns the size of the tileset in grid space
	const sf::Vector2i &getSize() const;
	// Returns the size of the tiles
	const sf::Vector2i &getTilesize() const;

	const std::vector<Frame> * const getAnimationFrames(int localId) const;

	sf::IntRect getTextureRect(int localId) const;

private:
	std::string m_name;
	int m_columns;
	int m_firstgid;
	sf::Vector2i m_tilesize;
	sf::Vector2i m_gridsize;
	sf::Texture m_texture;
	std::unordered_map<int, std::vector<Frame>> m_animations;
};
