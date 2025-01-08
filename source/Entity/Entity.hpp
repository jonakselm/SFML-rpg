#pragma once

class Entity
{
public:
	Entity() = default;
	~Entity() = default;

	virtual bool load(const std::string &filename, const sf::Vector2f &tileSize);

	virtual void update(const sf::Time &elapsedTime) = 0;

	virtual void draw(sf::RenderTarget &target) const = 0;

protected:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f m_size;
};