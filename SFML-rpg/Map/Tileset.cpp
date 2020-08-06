#include "stdafx.h"
#include "Tileset.hpp"

Tileset::Tileset()
{
}

Tileset::~Tileset()
{
}

void Tileset::load(const Json::Value &root)
{
	imagePath = "data/maps/" + root["image"].asString();

	if (!texture.loadFromFile(imagePath))
		exit(99);

	mapSize = sf::Vector2i(root["imagewidth"].asInt(), root["imageheight"].asInt());
	tileSize = sf::Vector2i(root["tilewidth"].asInt(), root["tileheight"].asInt());

	columns = root["columns"].asInt();
	tileCount = root["tilecount"].asInt();

	margin = root["margin"].asInt();
	spacing = root["spacing"].asInt();

	firstgid = root["firstgid"].asInt();

	name = root["name"].asString();

	for (unsigned int i = 0; i < root["tiles"].size(); i++)
	{
		Json::Value val = root["tiles"][i];
		AnimatedTile aniTile;
		aniTile.id = val["id"].asInt();
		Json::Value aniVal = val["animation"];
		for (unsigned int j = 0; j < aniVal.size(); j++)
		{
			aniTile.animation.push_back({ aniVal[j]["duration"].asInt64(), aniVal[j]["tileid"].asInt() });
		}
		animatedTiles.push_back(aniTile);
	}
}
