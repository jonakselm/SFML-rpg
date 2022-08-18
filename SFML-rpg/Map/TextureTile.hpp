#pragma once

class TextureTile : public sf::Drawable, public sf::Transformable
{
public:
	TextureTile();
	~TextureTile();

	void update();

	void setTexture(const sf::Texture &texture);
	void setTextureRect(const sf::IntRect &textureRect);

	const sf::Texture &getTexture() const;
	sf::IntRect getTextureRect() const;

private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	static constexpr int EDGE_COUNT = 4;
	std::array<sf::Vertex, EDGE_COUNT> m_vertices;
	sf::VertexBuffer m_buffer;
	const sf::Texture *m_texture = nullptr;
};