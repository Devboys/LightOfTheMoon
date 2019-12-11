/*
 * Created by Francesco Frassineti on 07/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [11/12/19] - Added simple sprite offset.
 * + Francesco Frassineti [07/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"
#include "Animation.hpp"

class AnimatorComponent : public Component {
public:
	explicit AnimatorComponent(GameObject* gameObject);

	void update(float deltaTime) override;

	void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) override;

	
	void setAnimation(std::shared_ptr<Animation> animation, bool forceOverride);//If you do not set "forceOverride" to true the animator will check if you're trying to set the same animation. If that's the case, the animator will not reassign it
	std::shared_ptr<Animation> getAnimation();

	void setOffset(glm::vec2 offset);

private:
	std::shared_ptr<Animation> animation;
	glm::vec2 offset = glm::vec2(0, 0);

	float timer = 0;
};