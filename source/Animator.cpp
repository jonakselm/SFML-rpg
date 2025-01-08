#include "stdafx.h"
#include "Animator.hpp"
#include <exception>
#include <utility>
#include <memory>

void Animator::addTile(Animation &&anim)
{
	m_animations.emplace_back(std::make_unique<Animation>(std::move(anim)));
	auto &a = *m_animations.back();
	a.tile.setTextureRect(a.frames[0].textureRect);
}

void Animator::update(const sf::Time &dt)
{
	for (int i = 0; i < m_animations.size(); i++)
	{
		auto &anim = m_animations.at(i);
		if (anim.get())
		{
			anim->timeSinceUpdate += dt;
			if (anim->timeSinceUpdate > anim->frames[anim->currFrame].dur)
			{
				anim->timeSinceUpdate -= anim->frames[anim->currFrame].dur;
				++anim->currFrame %= anim->frames.size();
				anim->tile.setTextureRect(anim->frames[anim->currFrame].textureRect);
			}
		}
		else
		{
			std::stringstream ss; 
			ss << "unique_ptr in animator was nullptr at index: " << i;
			throw std::runtime_error(ss.str());
		}
	}
}
