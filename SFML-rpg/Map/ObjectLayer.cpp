#include "stdafx.h"
#include "ObjectLayer.hpp"

void ObjectLayer::load(const Json::Value root, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets, const sf::Vector2i &mapSize)
{
	Layer::load(root);

	for (unsigned int i = 0; i < root["objects"].size(); i++)
	{
		Json::Value val = root["objects"][i];
		Object object;
		object.id = val["id"].asInt();
		object.pos = { val["x"].asFloat(), val["y"].asFloat() };
		TileTemplate &temp = object.tileTemplate;
		temp.load(val, tilesets);

		// Tiled starts from bottom left (needed top left)
		float top = object.pos.y - temp.templateSize.y;
		sf::FloatRect rect = { object.pos.x, top, temp.templateSize.x, temp.templateSize.y };
		bool isTemplate = temp.type == "template";
		object.tile.load(temp.gid, *temp.pTileset, rect, isTemplate);

		m_objects.push_back(object);
	}
}

void ObjectLayer::update(const sf::Time &elapsedTime)
{
	for (auto &object : m_objects)
		object.tile.updateObject(elapsedTime);
}

void ObjectLayer::draw(sf::RenderTarget &target) const
{
	for (const auto &object : m_objects)
		object.tile.draw(target);
}
