#include "stdafx.h"
#include "Layer.hpp"

void Layer::load(const Json::Value &root, const std::string &layerGroup, const LayerDetails &mapDetails)
{
	properties.load(root, mapDetails);
	group = layerGroup;
}

unsigned int Layer::getTilesetIndex(int gid, const std::vector<std::variant<const TilebasedTileset, const ImageTileset>> &tilesets) const
{
	int tilesetIndex = 0;
	for (const auto &variantTileset : tilesets)
	{
		const GenericTileset *tileset = nullptr;
		if (std::holds_alternative<const TilebasedTileset>(variantTileset))
			tileset = &std::get<const TilebasedTileset>(variantTileset);
		else
			tileset = &std::get<const ImageTileset>(variantTileset);

		if (!(gid > tileset->firstgid + tileset->tileCount - 1))
		{
			return tilesetIndex;
		}
		tilesetIndex++;
	}
	return 0;
}
