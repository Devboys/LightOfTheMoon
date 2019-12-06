/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#pragma once
#include "Component.hpp"
#include "PhysicsComponent.hpp"

//ABSTRACT CLASS for a generic component that damages other gameobjects once they collide with it
class DamageComponent : public Component {
public:
	explicit DamageComponent(GameObject* gameObject);
	virtual ~DamageComponent();

	void onCollisionStart(PhysicsComponent *comp) override;

	void onCollisionEnd(PhysicsComponent *comp) override;

	virtual int getDamage() = 0;

};