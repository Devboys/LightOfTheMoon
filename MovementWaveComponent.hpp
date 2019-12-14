/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [13/12/19] - Structure creation of members and methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"

class MovementWaveComponent : public Component {
public:
	explicit MovementWaveComponent(GameObject* gameObject);

	// A wave movement can only be fully initialized, direction and velocity cannot change after movement start
	void initParameters(const glm::vec2 startPosition, float& directionAngleDeg, const float& velocity, const float& amplitude);

	void update(float deltaTime) override;

	// Getters
	const float getDirectionDeg();
	const float getVelocity();
	const float getwaveAmplitude();

private:
	glm::vec2 _startPosition;
	float _directionAngleDegrees;
	float _velocity;
	float _waveAmplitude;
	float _totalTime;
};