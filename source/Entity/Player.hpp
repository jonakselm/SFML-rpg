#pragma once
#include "Entity.hpp"
#include <filesystem>

struct Weapon
{
	// Used for the offset in the X axis when selecting the spritesheet
	enum class Style
	{
		None,
		Knife,
		Sword,
		Bow
	};
	// Used for the offset in the Y axis
	enum class State
	{
		Inactive,
		Active
	};

	Style style;
	State state;
};

class Player : public Entity
{
public:
	enum class Direction
	{
		Down,
		Left,
		Right,
		Up
	};
public:
	Player(const std::filesystem::path &sourcePath);
	~Player();

	void update(const sf::Time &elapsedTime);

	void setWeaponStyle(Weapon::Style weaponStyle, bool updateSpritesheet = true);
	void setWeaponState(Weapon::State weaponState, bool updateSpritesheet = true);

	void move(const sf::Vector2f &vel);
	void move(float velX, float velY);

	// Does nothing if a new weaponstyle or weaponstate hasn't been set
	// The only use is when the updateSpritesheet parameter has been set to false
	void updateSpritesheetRect(bool updateTexRect);
	void updateTextureRect();

	void draw(sf::RenderTarget &target) const;

private:
	static constexpr int animationInterval = 300;
	Weapon m_weapon;
	Direction m_dir;
	sf::IntRect m_spritesheetRect;
	sf::Time m_timer;
};