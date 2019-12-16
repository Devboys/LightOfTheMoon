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

class MovementSpiralComponent : public Component {
public:
	explicit MovementSpiralComponent(GameObject* gameObject);

	// A spiral movement can only be fully initialized, direction and velocity cannot change after movement start
	void initParameters(const glm::vec2& center, const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise = true);

	void update(float deltaTime) override;

	// Getters
	const glm::vec2 getCenter();
	const float getRadiusExpansionRate();
	const bool isClockwise();

private:
	glm::vec2 _center;
	float _radiusExpansionRate;
	float _radius;
	float _velocity;
	float _minVelocity;
	float _maxVelocity;
	float _velocityDampFactor;
	bool _clockwise;
	float _totalTime;
	float _rotation = 0;
	std::shared_ptr<PhysicsComponent> phys;
};