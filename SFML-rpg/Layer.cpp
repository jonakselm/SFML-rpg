#include "stdafx.h"
#include "Layer.hpp"

void Layer::load(const Json::Value &root)
{
	id = root["id"].asInt();
	type = root["type"].asString();
	data.reserve(root["data"].size());
	for (unsigned int i = 0; i < root["data"].size(); i++)
	{
		data.push_back(root["data"][i].asInt());
	}
	name = root["name"].asString();
	size = sf::Vector2i(root["width"].asInt(), root["height"].asInt());
	opacity = root["opacity"].asFloat();
	visible = root["visible"].asBool();
}
