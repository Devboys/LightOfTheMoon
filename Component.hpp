/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Mouse handling
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#pragma once

#include "sre/SpriteBatch.hpp"

// Forward declaration
class GameObject;
class PhysicsComponent;

class Component {                                           // Abstract class that adds behavior to a GameObject
public:
	explicit Component(GameObject* gameObject);
	virtual ~Component() = default;

	GameObject* getGameObject();

	virtual bool onKey(SDL_Event& event);                   // The function should return true, if the key event is consumed. This prevents other components to receive the event.
	virtual void onMouse(SDL_Event &event);

	virtual void update(float deltaTime);
	virtual void renderSprite(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

	virtual void onCollisionStart(PhysicsComponent* comp);
	virtual void onCollisionEnd(PhysicsComponent* comp);
protected:
	GameObject* gameObject;

	friend class GameObject;
};