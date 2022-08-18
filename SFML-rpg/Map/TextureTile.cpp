#include "stdafx.h"
#include "TextureTile.hpp"
#include <iostream>
#include <chrono>

TextureTile::TextureTile()
{
	m_buffer.create(EDGE_COUNT);
	m_buffer.setPrimitiveType(sf::Quads);
}

TextureTile::~TextureTile()
{
	m_texture = nullptr;
}

void TextureTile::update()
{
	m_buffer.update(m_vertices.data());
}

void TextureTile::setTexture(const sf::Texture &texture)
{
	m_texture = &texture;
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

	target.draw(m_buffer, states);
}
