#include "stdafx.h"
#include "TextureTile.hpp"
#include <iostream>
#include <chrono>

TextureTile::TextureTile()
{
}

TextureTile::~TextureTile()
{
	m_texture = nullptr;
}

void TextureTile::update()
{
}

void TextureTile::setTexture(const sf::Texture &texture)
{
	m_texture = &texture;

	// Top left
	m_vertices[0].texCoords = sf::Vector2f(0, 0);
	// Top right
	m_vertices[1].texCoords = sf::Vector2f(float(texture.getSize().x), 0);
	// Bottom right
	m_vertices[2].texCoords = sf::Vector2f(texture.getSize());
	// Bottom left
	m_vertices[3].texCoords = sf::Vector2f(0, float(texture.getSize().y));

	// Top left
	m_vertices[0].position = sf::Vector2f(0, 0);
	// Top right
	m_vertices[1].position = sf::Vector2f(float(texture.getSize().x), 0);
	// Bottom right
	m_vertices[2].position = sf::Vector2f(texture.getSize());
	// Bottom left
	m_vertices[3].position = sf::Vector2f(0, float(texture.getSize().y));
}

void TextureTile::setTextureRect(const sf::IntRect &textureRect)
{
	int textureRight = textureRect.left + textureRect.width,
		textureBottom = textureRect.top + textureRect.height;

	// Top left
	m_vertices[0].texCoords = sf::Vector2f(float(textureRect.left), float(textureRect.top));
	// Top right
	m_vertices[1].texCoords = sf::Vector2f(float(textureRight), float(textureRect.top));
	// Bottom right
	m_vertices[2].texCoords = sf::Vector2f(float(textureRight), float(textureBottom));
	// Bottom left
	m_vertices[3].texCoords = sf::Vector2f(float(textureRect.left), float(textureBottom));

	// Top left
	m_vertices[0].position = sf::Vector2f(0, 0);
	// Top right
	m_vertices[1].position = sf::Vector2f(float(textureRect.width), 0);
	// Bottom right
	m_vertices[2].position = sf::Vector2f(float(textureRect.width), float(textureRect.height));
	// Bottom left
	m_vertices[3].position = sf::Vector2f(0, float(textureRect.height));
}

const sf::Texture &TextureTile::getTexture() const
{
	return *m_texture;
}

sf::IntRect TextureTile::getTextureRect() const
{
	// m_vertices[0] contains the top left position
	// m_vertices[2] contains the bottom right position
	return sf::IntRect(sf::Vector2i(m_vertices[0].texCoords),
		sf::Vector2i(m_vertices[2].texCoords - m_vertices[0].texCoords));
}

void TextureTile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = m_texture;
	states.transform *= getTransform();

	target.draw(m_vertices.data(), EDGE_COUNT, sf::Quads, states);
}
