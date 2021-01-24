#include "stdafx.h"
#include "Layer.hpp"

void Layer::load(const Json::Value &root, const std::string &layerGroup, const LayerDetails &mapDetails)
{
	properties.load(root, mapDetails);
	group = layerGroup;
}
