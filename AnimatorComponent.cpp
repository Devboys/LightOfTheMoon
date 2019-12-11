/*
 * Created by Francesco Frassineti on 07/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [07/12/19] - Basic creation
 */

#include "AnimatorComponent.hpp"
#include "GameObject.hpp"

AnimatorComponent::AnimatorComponent(GameObject* gameObject) : Component(gameObject){}

void AnimatorComponent::update(float deltaTime) {
	timer += deltaTime;
}

void AnimatorComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {
	sre::Sprite sprite = animation->getFrame(timer);
	sprite.setPosition(gameObject->getPosition() + offset);
	sprite.setRotation(gameObject->getRotation());
	spriteBatchBuilder.addSprite(sprite);
}

void AnimatorComponent::setAnimation(std::shared_ptr<Animation> animation, bool forceOverride) {
	if (forceOverride || animation != this->animation) {
		timer = 0;
		this->animation = animation;
	}
}

std::shared_ptr<Animation> AnimatorComponent::getAnimation() {
	return animation;
}

void AnimatorComponent::setOffset(glm::vec2 offset) {
	this->offset = offset;
}