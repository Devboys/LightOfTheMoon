/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Added member startinPosition to handle offset
 * + Alberto Giudice [14/12/19] - Revision of class structure
 * + Alberto Giudice [13/12/19] - Structure creation of members and methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "PhysicsComponent.hpp"

class MovementWaveComponent : public Component {
public:
	explicit MovementWaveComponent(GameObject* gameObject);

	// A wave movement can only be fully initialized, direction and velocity cannot change after movement start
	void initParameters(const glm::vec2 & startingPos, const float& directionAngleDeg, const float& velocity, const float& amplitude = 1.0f, const float& frequency = 1.0f);

	void update(float deltaTime) override;

	// Getters
	const float getDirectionDeg();
	const float getVelocity();
	const float getWaveAmplitude();
	const float getWaveFrequency();

private:
	glm::vec2 _startingPosition;
	float _directionAngleDegrees;
	float _velocity;
	float _frequency;
	float _amplitude;
	float _totalTime;
	std::shared_ptr<PhysicsComponent> phys;
};