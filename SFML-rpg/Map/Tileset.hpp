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

class ImageTile
{
public:
	void load(const Json::Value &root);

	std::string path;
	sf::Texture texture;
	unsigned int id;
	sf::Vector2f imageSize;
};

class GenericTileset
{
public:
	GenericTileset() = default;
	virtual ~GenericTileset() = default;

	virtual void load(const Json::Value &root) = 0;

	std::string name;
	std::vector<AnimatedTile> animatedTiles;
	int firstgid;
	int tileCount;
	int margin, spacing;
	sf::Vector2f tileSize;
};

class TilebasedTileset : public GenericTileset
{
public:
	TilebasedTileset() = default;
	~TilebasedTileset() = default;

	void load(const Json::Value & root);

	std::string imageSource;
	sf::Texture texture;
	sf::Vector2i mapSize;
	int columns = 0;
};

class ImageTileset : public GenericTileset
{
public:
	ImageTileset() = default;
	~ImageTileset() = default;

	void load(const Json::Value &root);

	std::vector<ImageTile> imageTiles;
};

class TileTemplate
{
public:
	TileTemplate() = default;
	~TileTemplate();

	void load(const Json::Value &root, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets);

	int gid;
	std::string name;
	std::string templateSource;
	sf::Vector2f templateSize;
	float rotation;
	std::string type;
	bool visible;
	const ImageTileset *pTileset = nullptr;
};