#pragma once
#include "Tileset.hpp"

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	// Messing with std::variant, i may fix it some other time
	/*Layer(const Layer &) = delete;
	Layer(Layer &) = delete;
	Layer &operator=(const Layer &) = delete;
	Layer &operator=(Layer &) = delete;*/

	virtual void update(const sf::Time &elapsedTime) = 0;

	virtual void load(const Json::Value &root, const std::string &group, const LayerDetails &mapDetails);

	virtual void draw(sf::RenderTarget &target) const = 0;

	LayerProperties properties;
	std::string group;

protected:
};