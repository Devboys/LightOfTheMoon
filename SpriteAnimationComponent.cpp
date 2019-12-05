/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "SpriteAnimationComponent.hpp"
#include "GameObject.hpp"
#include <memory>

SpriteAnimationComponent::SpriteAnimationComponent(GameObject* gameObject) : Component(gameObject) {}

void SpriteAnimationComponent::update(float deltaTime) {
	auto spriteComponent = gameObject->getComponent<SpriteComponent>();

	assert(spriteComponent != nullptr);

	time += deltaTime;

	if (time > animationTime) {
		time = fmod(time, animationTime);
		spriteIndex = (spriteIndex + 1) % sprites.size();
		spriteComponent->setSprite(sprites[spriteIndex]);
	}
}

void SpriteAnimationComponent::setSprites(std::vector<sre::Sprite> sprites) {
	this->sprites = sprites;
}

float SpriteAnimationComponent::getAnimationTime() const {
	return animationTime;
}

void SpriteAnimationComponent::setAnimationTime(float animationTime) {
	SpriteAnimationComponent::animationTime = animationTime;
}