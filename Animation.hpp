/*
 * Created by Francesco Frassineti on 07/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [07/12/19] - Basic creation
 */

#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "sre/SpriteBatch.hpp"
#include <string>
#include "rapidjson/document.h"
#pragma once

class Animation{
public:
	explicit Animation(const std::vector<sre::Sprite>& sprites, const float& length, const bool& loop);

	std::vector<sre::Sprite> getSprites();
	const sre::Sprite getFrame(float time);

private:
	std::vector<sre::Sprite> sprites;
	float length = 1.0;
	bool loop = true;
};
