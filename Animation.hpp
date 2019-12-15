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
/*Class for an animation clip*/
class Animation{
public:
	explicit Animation(const std::vector<sre::Sprite>& sprites, const float& length, const bool& loop);

	std::vector<sre::Sprite> getSprites();/*The sequence of sprites*/
	const sre::Sprite getFrame(float time);/*Returns the frame at that time*/

private:
	std::vector<sre::Sprite> sprites;/*The sequence of sprites*/
	float length = 1.0;/*The duration of the animation clip*/
	bool loop = true;/*Does it loop when time > length or does it hold the last frame?*/
};
