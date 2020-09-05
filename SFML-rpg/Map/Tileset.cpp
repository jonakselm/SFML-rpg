#include "stdafx.h"
#include "Tileset.hpp"
#include <iostream>

void GenericTileset::load(const Json::Value &root)
{
	tileCount = root["tilecount"].asInt();

	margin = root["margin"].asInt();
	spacing = root["spacing"].asInt();

	firstgid = root["firstgid"].asInt();

	name = root["name"].asString();

	tileSize = sf::Vector2f(root["tilewidth"].asInt(), root["tileheight"].asInt());

	for (unsigned int i = 0; i < root["tiles"].size(); i++)
	{
		Json::Value tiles = root["tiles"][i];

		AnimatedTile aniTile;
		aniTile.id = tiles["id"].asInt();
		Json::Value aniVal = tiles["animation"];
		if (!aniVal.empty())
		{
			for (unsigned int j = 0; j < aniVal.size(); j++)
			{
				aniTile.animation.push_back({ aniVal[j]["duration"].asInt64(), aniVal[j]["tileid"].asInt() });
			}
			animatedTiles.push_back(aniTile);
		}
	}
}

void TilebasedTileset::load(const Json::Value &root)
{
	GenericTileset::load(root);

	imageSource = "data/maps/" + root["image"].asString();
	mapSize = sf::Vector2i(root["imagewidth"].asInt(), root["imageheight"].asInt());
	columns = root["columns"].asInt();

	if (!texture.loadFromFile(imageSource))
	{
		std::cout << std::endl << "---------------------------------------" << std::endl;
		std::cout << "   Encountered error loading texture   " << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		exit(-1);
	}
}

void ImageTileset::load(const Json::Value &root)
{
	GenericTileset::load(root);

	for (unsigned int i = 0; i < root["tiles"].size(); i++)
	{
		Json::Value tiles = root["tiles"][i];

		if (!tiles["image"].empty())
		{
			ImageTile imageTile;
			imageTile.load(tiles);
			imageTiles.push_back(imageTile);
		}
	}
}

TileTemplate::~TileTemplate()
{
	pTileset = nullptr;
}

void TileTemplate::load(const Json::Value &root, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets)
{
	Json::Value templateRoot;
	if (!root["template"].empty())
	{
		templateSource = "data/maps/" + root["template"].asString();

		std::ifstream file(templateSource);

		file >> templateRoot;
	}
	else
		templateRoot = root;

	type = templateRoot["type"].asString();
	Json::Value object;
	if (type == "template")
		object = templateRoot["object"];
	else
		object = templateRoot;

	gid = object["gid"].asInt();
	name = object["gid"].asString();
	templateSize = sf::Vector2f(object["width"].asFloat(), object["height"].asFloat());
	rotation = object["rotation"].asFloat();
	visible = object["visible"].asBool();
	for (const auto &tileset : tilesets)
	{
		if (!dynamic_cast<const ImageTileset *>(tileset.get()))
			continue;
		std::string path;
		Json::Value tilesetRoot;
		if (!root["template"].empty())
		{
			path = templateSource + "/../" + templateRoot["tileset"]["source"].asString();
			std::ifstream file(path);
			file >> tilesetRoot;
		}
		else
		{
			tilesetRoot = root;
		}

		if (tileset->name == tilesetRoot["name"].asString())
		{
			pTileset = dynamic_cast<const ImageTileset *>(tileset.get());
		}
		else if (gid >= tileset->firstgid && gid <= tileset->firstgid + tileset->tileCount)
		{
			pTileset = dynamic_cast<const ImageTileset *>(tileset.get());
		}
	}
}

void ImageTile::load(const Json::Value &root)
{
	id = root["id"].asInt();
	path = "data/maps/" + root["image"].asString();
	imageSize = { root["imagewidth"].asFloat(), root["imageheight"].asFloat() };
	if (!texture.loadFromFile(path))
	{
		std::cout << std::endl << "---------------------------------------" << std::endl;
		std::cout << "   Encountered error loading texture   " << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		exit(-1);
	}
}
