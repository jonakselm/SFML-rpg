#pragma once

class Animation
{
public:
	__int64 duration;
	int tileId;
};

class AnimatedTile
{
public:
	std::vector<Animation> animation;
	unsigned int id;
};

// TODO: make animatedTiles being used, and updating
class Tileset
{
public:
	Tileset();
	~Tileset();

	void load(const Json::Value &root);

	std::shared_ptr<sf::Texture> texture;
	std::string imagePath;
	std::string name;
	sf::Vector2i tileSize, mapSize;
	std::vector<AnimatedTile> animatedTiles;
	int firstgid;
	int tileCount, columns;
	int margin, spacing;
};