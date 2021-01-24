#include "stdafx.h"
#include "ObjectLayer.hpp"

void ObjectLayer::update(const sf::Time &elapsedTime)
{
	for (auto &object : m_objects)
		object.tile.updateObject(elapsedTime);
}

void ObjectLayer::load(const Json::Value root, const std::string &layerGroup, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets, const LayerDetails &mapDetails)
{
	Layer::load(root, layerGroup, mapDetails);

	for (unsigned int i = 0; i < root["objects"].size(); i++)
	{
		Json::Value val = root["objects"][i];
		ObjectTile objTile;
		if (val["template"].empty())
		{
			Object object;
			object.load(val, tilesets);
			objTile.object.emplace<Object>(object);
		}
		else
		{
			TemplateObject object;
			object.load(val, tilesets);
			objTile.object.emplace<TemplateObject>(object);
		}

		if (std::holds_alternative<TemplateObject>(objTile.object))
			objTile.tile.load(std::get<TemplateObject>(objTile.object), properties.opacity);
		else
			objTile.tile.load(std::get<Object>(objTile.object), properties.opacity);

		m_objects.push_back(std::move(objTile));
	}
}

void ObjectLayer::draw(sf::RenderTarget &target) const
{
	for (auto &object : m_objects)
		object.tile.draw(target);
}
