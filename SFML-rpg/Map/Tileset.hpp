#pragma once
#include "MapUtility.hpp"

struct TileProperties
{
	unsigned int id = 0;
	Animation animation;
	LayerProperties layerProps;
	std::vector<ObjectProperties> collisionObjects;
};

struct ImageTile
{
	void load(const Json::Value &root);

	std::string path;
	sf::Texture texture;
	unsigned int id = 0;
	sf::Vector2f imageSize;
};

class GenericTileset
{
public:
	GenericTileset() = default;
	virtual ~GenericTileset() = default;

	// Messing with my use of std::variant, might uncomment when i find another solution
	/*GenericTileset(const GenericTileset &) = delete;
	GenericTileset(GenericTileset &) = delete;
	GenericTileset &operator=(const GenericTileset &) = delete;
	GenericTileset &operator=(GenericTileset &) = delete;*/

	virtual void load(const Json::Value &root) = 0;

	std::string name;
	std::vector<TileProperties> tileProperties;
	int firstgid = 0;
	int tileCount = 0;
	int margin = 0, spacing = 0;
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

class Object
{
public:
	Object() = default;
	~Object() = default;
	virtual void load(const Json::Value &root, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets);

protected:
	// Doesn't load tilesets. Made for TemplateObject, since it has
	// easier tileset deduction.
	virtual void load(const Json::Value &root);
public:
	ObjectProperties objProps;
	int gid = 0;
	const std::variant<const TilebasedTileset, const ImageTileset> *pTileset = nullptr;
	bool tilebased = false;
};

class TemplateObject : public Object
{
public:
	TemplateObject() = default;
	~TemplateObject() = default;
	void load(const Json::Value &root, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets) override;

	int id = 0;
	std::string templatePath;
	std::string tilesetPath;
	std::string type;
};