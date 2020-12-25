#pragma once
#include <SFML/Graphics.hpp>
#include <json/json.h>

struct LayerDetails
{
	sf::Vector2i tileSize = { 0, 0 };
	int width = 0;
	int height = 0;
};

struct AnimationFrame
{
	__int64 duration = 0;
	int tileId = 0;
};

struct Animation
{
	std::vector<AnimationFrame> frames;
};

class ObjectProperties
{
public:
	void load(const Json::Value &root)
	{
		id = root["id"].asInt();
		name = root["name"].asString();
		type = root["type"].asString();
		rotation = root["rotation"].asFloat();
		// Falls back on its own value if there are no x and y coordinate,
		// because the x and y may be elsewhere and therefore already initialized.
		// If there are no values loaded, they are initialized with standard values of 0
		sf::Vector2f pos = { rect.left, rect.top };
		if (!root["x"].empty())
			pos.x = root["x"].asFloat();
		if (!root["y"].empty())
			pos.y = root["y"].asFloat();
		rect = sf::FloatRect(pos.x, pos.y,
			root["width"].asFloat(), root["height"].asFloat());
		visible = root["visible"].asBool();
	}

	int id = 0;
	std::string name;
	float rotation = 0;
	std::string type;
	sf::FloatRect rect;
	bool visible = false;
};

class LayerProperties
{
public:
	void load(const Json::Value &root)
	{
		id = root["id"].asInt();
		name = root["name"].asString();
		type = root["type"].asString();
		opacity = root["opacity"].asFloat();
		visible = root["visible"].asBool();
	}
	void load(const Json::Value &root, const LayerDetails &mapDetails)
	{
		id = root["id"].asInt();
		name = root["name"].asString();
		type = root["type"].asString();
		layerDetails = mapDetails;
		opacity = root["opacity"].asFloat();
		visible = root["visible"].asBool();
	}

	int id = 0;
	std::string type;
	std::string name;
	LayerDetails layerDetails;
	float opacity = 0;
	bool visible = false;
};