#include "TextureTile.hpp"
#include <Utility.hpp>
#include <iostream>

struct Animation
{
	Animation() = default;
	Animation(TextureTile &tile, const std::vector<Frame> &frames, int currFrame, sf::Time timeSinceUpdate)
		: tile(tile), frames(frames), currFrame(currFrame), timeSinceUpdate(timeSinceUpdate) {}
	//Animation(Animation& anim) : tile(anim.tile), frames(anim.frames) { std::cout << "animation was copied" << std::endl; }
	//Animation(Animation&& anim) : tile(anim.tile), frames(anim.frames) { std::cout << "animation was moved" << std::endl; }

	TextureTile &tile;
	const std::vector<Frame> frames;
	int currFrame = 0;
	sf::Time timeSinceUpdate;
};

class Animator
{
public:
	Animator() = default;
	~Animator() = default;

	void addTile(Animation &&anim);
	
	void update(const sf::Time &dt);

private:
	std::vector<std::unique_ptr<Animation>> m_animations;
};

