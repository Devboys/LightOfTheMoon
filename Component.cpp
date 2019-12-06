/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Mouse Handling
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "Component.hpp"

Component::Component(GameObject* gameObject)
	:gameObject(gameObject){

}

void Component::renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {

}

void Component::update(float deltaTime) {

}

GameObject* Component::getGameObject() {
	return gameObject;
}

bool Component::onKey(SDL_Event& event) {
	return false;
}

void Component::onMouse(SDL_Event &event) {
	
}

void Component::onCollisionStart(PhysicsComponent* comp) {

}

void Component::onCollisionEnd(PhysicsComponent* comp) {

}