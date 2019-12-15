/*
 * Created by Francesco Frassineti on 07/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [07/12/19] - Basic creation
 */

#include <SDL_events.h>
#include <iostream>
#include "Animation.hpp"

Animation::Animation(const std::vector<sre::Sprite>& sprites, const float& length, const bool& loop) {
	this->sprites = sprites;
	this->length = length;
	this->loop = loop;
}

std::vector<sre::Sprite> Animation::getSprites() {
	return this->sprites;
}

const sre::Sprite Animation::getFrame(float time) {
	{
		if (loop) {
			time = fmod(time, length);
		}
		else {
			time = glm::min(time, length);
		}

		return sprites[(int)(sprites.size() * time / length)];
	}
}


