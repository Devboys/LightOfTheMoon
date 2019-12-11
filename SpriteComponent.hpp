/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [11/12/19] - Added simple sprite offset.
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "sre/Sprite.hpp"

class SpriteComponent : public Component {
public:
	explicit SpriteComponent(GameObject* gameObject);

	void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) override;

	void setSprite(const sre::Sprite& sprite);
	sre::Sprite getSprite();

	void setOffset(glm::vec2 offset);

	void flipSpriteHorizontal(const bool& left);

private:
	sre::Sprite sprite;
	glm::vec2 offset = glm::vec2(0, 0);
};