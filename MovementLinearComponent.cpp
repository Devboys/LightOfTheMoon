/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Decoupled position of gameobject to physics, due to bugs
 * + Alberto Giudice [15/12/19] - Fixed the overloaded call from angle to vector
 * + Francesco Frassineti [14/12/19] - Overload of initParameters to support vectors
 * + Alberto Giudice [14/12/19] - Implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementLinearComponent.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameObject.hpp"
#include "LightOfTheMoon.hpp"

MovementLinearComponent::MovementLinearComponent(GameObject* gameObject) : Component(gameObject) {}

/* Initialize the parameters of a linear movement
 *
 * @param directionAngleDeg : direction the attached gameObject will travel to, in degrees
 * @param velocity : constant speed the attached gameObject should have in its linear trajectory
 */
void MovementLinearComponent::initParameters(const float& directionAngleDeg, const float& velocity) {
	_directionAngleDegrees = directionAngleDeg;
	_velocity = velocity;

	phys = gameObject->getComponent<PhysicsComponent>();
	if (phys != nullptr) {
		phys->setLinearVelocity(glm::vec2(_velocity * std::cos(_directionAngleDegrees * M_PI / 180), _velocity * std::sin(_directionAngleDegrees * M_PI / 180)));
	}
}

void MovementLinearComponent::update(float deltaTime) {

}

const float MovementLinearComponent::getDirectionDeg() {
	return _directionAngleDegrees;
}

const float MovementLinearComponent::getVelocity() {
	return _velocity;
}