/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - Implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementLinearComponent.hpp"

MovementLinearComponent::MovementLinearComponent(GameObject* gameObject) : Component(gameObject) {}

void MovementLinearComponent::initParameters(const float& directionAngleDeg, const float& velocity) {
	_directionAngleDegrees = directionAngleDeg;
	_velocity = velocity;
}

const float MovementLinearComponent::getDirectionDeg() {
	return _directionAngleDegrees;
}

const float MovementLinearComponent::getVelocity() {
	return _velocity;
}

// Logic for moving the object attached to this in a linear line over time
void MovementLinearComponent::update(float deltaTime) {
	
}