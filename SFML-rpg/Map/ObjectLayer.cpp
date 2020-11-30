#include "stdafx.h"
#include "ObjectLayer.hpp"

void ObjectLayer::load(const Json::Value root, const std::string &layerGroup, const std::vector<std::unique_ptr<const GenericTileset>> &tilesets, const sf::Vector2i &mapSize)
{
	Layer::load(root, layerGroup);

	for (unsigned int i = 0; i < root["objects"].size(); i++)
	{
		Json::Value val = root["objects"][i];
		std::unique_ptr<Object> pObject;
		pObject->id = val["id"].asInt();
		pObject->pos = { val["x"].asFloat(), val["y"].asFloat() };
		TileTemplate &tTemplate = pObject->tileTemplate;
		tTemplate.load(val, tilesets);

		sf::FloatRect rect = { pObject->pos, tTemplate.templateSize };
		bool isTemplate = tTemplate.type == "template";
		pObject->tile.load(tTemplate, rect, isTemplate);

		m_objects.push_back(std::move(pObject));
	}
}

void ObjectLayer::update(const sf::Time &elapsedTime)
{
	for (auto &pObject : m_objects)
		pObject->tile.updateObject(elapsedTime);
}

void ObjectLayer::draw(sf::RenderTarget &target) const
{
	for (const auto &pObject : m_objects)
		pObject->tile.draw(target);
}
}
