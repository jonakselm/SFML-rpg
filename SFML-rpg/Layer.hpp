#pragma once

class Layer
{
public:
	Layer() = default;
	virtual ~Layer() = default;

	virtual void load(const Json::Value &root);

	virtual void draw(sf::RenderTarget &target) const = 0;

	virtual void update(const sf::Time &time) = 0;

	int id;
	std::string type;
	std::vector<int> data;
	std::string name;
	sf::Vector2i size;
	float opacity;
	bool visible;
};