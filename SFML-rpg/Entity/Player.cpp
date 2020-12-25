#include "stdafx.h"
#include "Player.hpp"

Player::Player()
	:
	m_weapon{Weapon::Style::None, Weapon::State::Inactive},
	m_dir(Direction::Right)
{
	m_size = { 32, 32 };

	m_texture.loadFromFile("data/player sprites/playerSprite_32x32.png");
	m_sprite.setTexture(m_texture);
}

Player::~Player()
{
}

void Player::update(const sf::Time &elapsedTime)
{
}

void Player::setWeaponStyle(Weapon::Style weaponStyle, bool updateSpritesheet)
{
	m_weapon.style = weaponStyle;
	if (updateSpritesheet)
		updateSpritesheetRect(true);
}

void Player::setWeaponState(Weapon::State weaponState, bool updateSpritesheet)
{
	m_weapon.state = weaponState;
	if (updateSpritesheet)
		updateSpritesheetRect(true);
}

void Player::move(const sf::Vector2f &vel)
{
	m_sprite.move(vel);
}

void Player::move(float velX, float velY)
{
	move(sf::Vector2f(velX, velY));
}

void Player::updateSpritesheetRect(bool updateTexRect)
{
	const int x = static_cast<int>(m_weapon.style);
	const int y = static_cast<int>(m_weapon.state);

	// 4x4 spritesheets
	const int width = int(m_size.x) * 4;
	const int height = int(m_size.y) * 4;
	int left = x * width;
	int top = y * height;

	m_spritesheetRect = sf::IntRect(left, top, width, height);

	if (updateTexRect)
		updateTextureRect();
}

void Player::updateTextureRect()
{
}

void Player::draw(sf::RenderTarget &target) const
{
	target.draw(m_sprite);
}
