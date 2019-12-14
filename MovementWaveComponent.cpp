/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - First implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementWaveComponent.hpp"

MovementWaveComponent::MovementWaveComponent(GameObject* gameObject) : Component(gameObject) {}

void MovementWaveComponent::initParameters(const glm::vec2 startPosition, float& directionAngleDeg, const float& velocity, const float& amplitude) {
	_startPosition = startPosition;
	_directionAngleDegrees = directionAngleDeg;
	_velocity = velocity;
	_waveAmplitude = amplitude;
	_totalTime = 0.0f;
}

const float MovementWaveComponent::getDirectionDeg() {
	return _directionAngleDegrees;
}

const float MovementWaveComponent::getVelocity() {
	return _velocity;
}

const float MovementWaveComponent::getwaveAmplitude() {
	return _waveAmplitude;
}

// Logic for moving the object attached to this in a sine wave line over time
void MovementWaveComponent::update(float deltaTime) {

}