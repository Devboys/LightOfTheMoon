/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "SpriteComponent.hpp"
#include "GameObject.hpp"

SpriteComponent::SpriteComponent(GameObject* gameObject) : Component(gameObject) {}

void SpriteComponent::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {
	sprite.setPosition(gameObject->getPosition() + offset);
	sprite.setRotation(gameObject->getRotation());
	spriteBatchBuilder.addSprite(sprite);
}

void SpriteComponent::setSprite(const sre::Sprite& sprite) {
	this->sprite = sprite;
}

sre::Sprite SpriteComponent::getSprite() {
	return sprite;
}

void SpriteComponent::setOffset(glm::vec2 offset) {
	this->offset = offset;
}

void SpriteComponent::flipSpriteHorizontal(const bool& left) {
	if (left)
		sprite.setFlip(glm::bvec2(true, false));
	else
		sprite.setFlip(glm::bvec2(false, false));
}