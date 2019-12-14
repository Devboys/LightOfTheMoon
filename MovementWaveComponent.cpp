/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Logic implementation of the sin wave movement
 * + Alberto Giudice [14/12/19] - First implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementWaveComponent.hpp"
#include "GameObject.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "LightOfTheMoon.hpp"

MovementWaveComponent::MovementWaveComponent(GameObject* gameObject) : Component(gameObject) {}

void MovementWaveComponent::initParameters(const float& directionAngleDeg, const float& velocity, const float& amplitude, const float& frequency) {
	_directionAngleDegrees = directionAngleDeg;
	_velocity = velocity;
	_frequency = frequency;
	_amplitude = amplitude;
	_totalTime = 0.0f;

	phys = gameObject->getComponent<PhysicsComponent>();
}

const float MovementWaveComponent::getDirectionDeg() {
	return _directionAngleDegrees;
}

const float MovementWaveComponent::getVelocity() {
	return _velocity;
}

const float MovementWaveComponent::getWaveAmplitude() {
	return _amplitude;
}

const float MovementWaveComponent::getWaveFrequency() {
	return _frequency;
}

// Logic for moving the object attached to this in a sine wave line over time
void MovementWaveComponent::update(float deltaTime) {
	glm::vec2 newPos;
	// move position based on the sin wave function
	newPos.x = _totalTime * _velocity;
	newPos.y = _amplitude * std::sin(_frequency * _totalTime);

	// position rotation by the required degrees
	float newPosX = newPos.x;
	float newPosY = newPos.y;
	newPos.x = std::cos(_directionAngleDegrees * M_PI / 180) * newPosX - std::sin(_directionAngleDegrees * M_PI / 180) * newPosY;
	newPos.y = std::sin(_directionAngleDegrees * M_PI / 180) * newPosX + std::cos(_directionAngleDegrees * M_PI / 180) * newPosY;

	// set the position of the bullet gameObject to the new position
	gameObject->setPosition(newPos);

	if (phys != nullptr) {
		phys->moveTo(newPos/LightOfTheMoon::physicsScale);
	}

	_totalTime += deltaTime;
}