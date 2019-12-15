/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Fixed the overloaded call from angle to vector
 * + Francesco Frassineti [14/12/19] - Overload of initParameters to support vectors
 * + Alberto Giudice [14/12/19] - Implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementLinearComponent.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameObject.hpp"

MovementLinearComponent::MovementLinearComponent(GameObject* gameObject) : Component(gameObject) {}

void MovementLinearComponent::initParameters(const float& directionAngleDeg, const float& velocity) {
	_directionAngleDegrees = directionAngleDeg;
	initParameters({ std::cos(_directionAngleDegrees * M_PI / 180), std::sin(_directionAngleDegrees * M_PI / 180) }, velocity);
}

void MovementLinearComponent::initParameters(const glm::vec2& direction, const float& velocity) {
	_velocity = velocity;
	std::shared_ptr<PhysicsComponent> phys = gameObject->getComponent<PhysicsComponent>();
	if (phys != nullptr) {
		phys->setLinearVelocity(_velocity * direction);
	}
}

const float MovementLinearComponent::getDirectionDeg() {
	return _directionAngleDegrees;
}

const float MovementLinearComponent::getVelocity() {
	return _velocity;
}