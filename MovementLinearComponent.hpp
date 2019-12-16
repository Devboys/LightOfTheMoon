/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [13/12/19] - Structure creation of members and methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "PhysicsComponent.hpp"

class MovementLinearComponent : public Component {
public:
	explicit MovementLinearComponent(GameObject* gameObject);

	// A linear movement can only be fully initialized, direction and velocity cannot change after movement start
	void initParameters(const float& directionAngleDeg, const float& velocity);

	void update(float deltaTime) override;

	// Getters
	const float getDirectionDeg();
	const float getVelocity();

private:
	float _directionAngleDegrees;
	float _velocity;
	std::shared_ptr<PhysicsComponent> phys;
};