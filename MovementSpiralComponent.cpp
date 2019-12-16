/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - First implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include <iostream>
#include "MovementSpiralComponent.hpp"
#include "GameObject.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "LightOfTheMoon.hpp"

MovementSpiralComponent::MovementSpiralComponent(GameObject* gameObject) : Component(gameObject) {}

/* Initialize the parameters of a spiral movement
 *
 * @param center : The position around which the spiral will rotate
 * @param minVelocity : minimum angular speed the attached gameObject will reach over time
 * @param maxVelocity : initial angular speed when the attached gameObject is spawned. Will decrease over time
 * @param radiusExpansionRate : rate at which the radius of the spiral will increase over time
 * @param clockwise : true = clockwise rotation, false = counterclockwise rotation
 */
void MovementSpiralComponent::initParameters(const glm::vec2& center, const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise) {
	_center = center;
	_radiusExpansionRate = radiusExpansionRate;
	_radius = 0.001f;
	_minVelocity = minVelocity;
	_maxVelocity = maxVelocity;
	_velocity = _maxVelocity;
	_velocityDampFactor = _maxVelocity * 0.05f;
	_clockwise = clockwise;
	_totalTime = 0.0f;

	phys = gameObject->getComponent<PhysicsComponent>();
}

const glm::vec2 MovementSpiralComponent::getCenter() {
	return _center;
}

const float MovementSpiralComponent::getRadiusExpansionRate() {
	return _radiusExpansionRate;
}

const bool MovementSpiralComponent::isClockwise() {
	return _clockwise;
}

// Logic for moving the object attached to this in spiral stemming from the _center over time
void MovementSpiralComponent::update(float deltaTime) {
	int rotDirection;
	if (_clockwise)
		rotDirection = 1;
	else
		rotDirection = -1;

	// Change rotation of the object, tangent to the next position on the spiral
	float oldRot = _rotation;
	_rotation = std::fmod((oldRot + rotDirection * _velocity * deltaTime), 360.0f);

	// Calculate new position with increased radius
	glm::vec2 newPos = _center;
	newPos.x += std::sin(_rotation * M_PI / 180.0f) * _radius;
	newPos.y += std::cos(_rotation * M_PI / 180.0f) * _radius;

	// Set the position of the bullet gameObject to the new position
	phys->moveTo(newPos / LightOfTheMoon::physicsScale);

	// Decrease velocity until minVelocity is reached, to prevent crazy speed on big radius values
	if(_velocity > _minVelocity)
		_velocity -= _velocityDampFactor * deltaTime;

	// Increase radius
	_radius += _radiusExpansionRate * deltaTime;
}