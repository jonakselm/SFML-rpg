#pragma once

class Tileset
{
public:
	Tileset();
	~Tileset();

	void load(const Json::Value &root);

	sf::Texture texture;
	std::string imagePath;
	std::string name;
	sf::Vector2i tileSize, mapSize;
	int firstgid;
	int tileCount, columns;
	int margin, spacing;
};