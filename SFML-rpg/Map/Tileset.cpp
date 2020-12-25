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

	tileSize = sf::Vector2f(root["tilewidth"].asFloat(), root["tileheight"].asFloat());

	for (unsigned int i = 0; i < root["tiles"].size(); i++)
	{
		Json::Value tiles = root["tiles"][i];

		bool hasProps = false;
		TileProperties tileProps;
		tileProps.id = tiles["id"].asInt();
		Json::Value animationVal = tiles["animation"];
		Json::Value objectgroupVal = tiles["objectgroup"];
		if (!animationVal.empty())
		{
			for (unsigned int j = 0; j < animationVal.size(); j++)
			{
				tileProps.animation.frames.push_back({ animationVal[j]["duration"].asInt64(), animationVal[j]["tileid"].asInt() });
			}
			tileProperties.push_back(tileProps);
		}
		if (!objectgroupVal.empty())
		{
			tileProps.layerProps.load(objectgroupVal);

			for (const auto &objectVal : objectgroupVal["objects"])
			{
				ObjectProperties object;
				object.load(objectVal);
				tileProps.collisionObjects.push_back(object);
			}
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
		std::cout << std::endl << "------------------------------------------------" << std::endl;
		std::cout << "   Encountered error loading TilebasedTileset   " << std::endl;
		std::cout << "------------------------------------------------" << std::endl;
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

void ImageTile::load(const Json::Value &root)
{
	id = root["id"].asInt();
	path = "data/maps/" + root["image"].asString();
	imageSize = { root["imagewidth"].asFloat(), root["imageheight"].asFloat() };
	if (!texture.loadFromFile(path))
	{
		std::cout << std::endl << "---------------------------------------" << std::endl;
		std::cout << "   Encountered error loading ImageTile   " << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		exit(-1);
	}
}

void Object::load(const Json::Value &root)
{
	objProps.load(root);
	gid = root["gid"].asInt();
}

void Object::load(const Json::Value &root, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets)
{
	// Find tileset based on gid (non template)
	// (Not a template and therefore currently using local directory)
	// "gid":468,|
	// "height" : 512,|
	// "id" : 15,|
	// "name" : "",|
	// "rotation" : 340.298007704893,|
	// "type" : "",|
	// "visible" : true,|
	// "width" : 512,|
	// "x" : 182.291308135998,|
	// "y" : 778.31822537361|
	///////////////////////////////////
	load(root);

	// Have to deduce tileset as it doesn't have the path stored
	for (const auto &variantTileset : tilesets)
	{
		const GenericTileset *tileset = nullptr;
		if (std::holds_alternative<const TilebasedTileset>(variantTileset))
		{
			tileset = &std::get<const TilebasedTileset>(variantTileset);
			tilebased = true;
		}
		else
		{
			tileset = &std::get<const ImageTileset>(variantTileset);
			tilebased = false;
		}

		if (gid >= tileset->firstgid && gid < tileset->firstgid + tileset->tileCount)
		{
			pTileset = &variantTileset;
			break;
		}
	}
}

void TemplateObject::load(const Json::Value &root, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets)
{
	// No need to deduce tileset as its relative path is under the template's category->[tileset]

	templatePath = "data/maps/" + root["template"].asString();
	// Contains object info, tileset info and template type
	Json::Value templateVal;
	std::ifstream templateFile(templatePath);
	templateFile >> templateVal;

	// Set x and y, as they are found in the objectlayer's objectlist
	objProps.rect = sf::FloatRect(root["x"].asFloat(), root["y"].asFloat(),
		objProps.rect.width, objProps.rect.height);
	id = root["id"].asInt();
	type = templateVal["type"].asString();
	const Json::Value objectVal = templateVal["object"];
	Object::load(objectVal);

	///////////////////////////////////////////////////////////////////////
	// Getting tileset

	// Contains tileset's source and firstgid
	const Json::Value tilesetInfoVal = templateVal["tileset"];
	// Contains info stored in the actual tileset (image source, etc.)
	Json::Value tilesetVal;
	// Getting the source for the tileset
	tilesetPath = templatePath + "/../" + tilesetInfoVal["source"].asString();
	std::ifstream tilesetFile(tilesetPath);
	tilesetFile >> tilesetVal;

	for (const auto &variantTileset : tilesets)
	{
		const GenericTileset *tileset = nullptr;
		if (std::holds_alternative<const TilebasedTileset>(variantTileset))
		{
			tileset = &std::get<const TilebasedTileset>(variantTileset);
			tilebased = true;
		}
		else
		{
			tileset = &std::get<const ImageTileset>(variantTileset);
			tilebased = false;
		}

		if (tileset->name == tilesetVal["name"].asString())
		{
			pTileset = &variantTileset;
			break;
		}
	}

	// End of getting tileset
	///////////////////////////////////////////////////////////////////////
}
