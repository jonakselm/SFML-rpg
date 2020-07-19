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
	texture.loadFromFile(imagePath);

	mapSize = sf::Vector2i(root["imagewidth"].asInt(), root["imageheight"].asInt());
	tileSize = sf::Vector2i(root["tilewidth"].asInt(), root["tileheight"].asInt());

	columns = root["columns"].asInt();
	tileCount = root["tilecount"].asInt();

	margin = root["margin"].asInt();
	spacing = root["spacing"].asInt();

	firstgid = root["firstgid"].asInt();

	name = root["name"].asString();
}
