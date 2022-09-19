#pragma once

class Tileset
{
public:
	void load(Json::Value root);

	void setFirstgid(int firstgid);
	int getFirstgid() const;

	const sf::Texture &getTexture() const;
	// Returns the size of the tileset in grid space
	const sf::Vector2i &getSize() const;
	// Returns the size of the tiles
	const sf::Vector2i &getTilesize() const;

private:
	std::string m_name;
	int m_columns;
	int m_firstgid;
	sf::Vector2i m_tilesize;
	sf::Vector2i m_gridsize;
	sf::Texture m_texture;
};