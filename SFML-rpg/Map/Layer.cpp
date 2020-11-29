#include "stdafx.h"
#include "Layer.hpp"

void Layer::load(const Json::Value &root, const std::string &layerGroup)
{
	id = root["id"].asInt();
	type = root["type"].asString();
	name = root["name"].asString();
	size = sf::Vector2i(root["width"].asInt(), root["height"].asInt());
	opacity = root["opacity"].asFloat();
	visible = root["visible"].asBool();
	group = layerGroup;
}

unsigned int Layer::getTilesetIndex(int gid, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets) const
{
	int tilesetIndex = 0;
	for (const auto &tileset : tilesets)
	{
		if (!(gid > tileset->firstgid + tileset->tileCount - 1))
		{
			return tilesetIndex;
		}
		tilesetIndex++;
	}
	return 0;
}
