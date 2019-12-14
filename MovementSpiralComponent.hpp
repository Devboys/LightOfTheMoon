/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [13/12/19] - Structure creation of members and methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"

class MovementSpiralComponent : public Component {
public:
	explicit MovementSpiralComponent(GameObject* gameObject);

	// A spiral movement can only be fully initialized, direction and velocity cannot change after movement start
	void initParameters(const glm::vec2 center, const float& radiusExpansionRate, const bool& clockwise);

	void update(float deltaTime) override;

	// Getters
	const glm::vec2 getCenter();
	const float getRadiusExpansionRate();
	const bool isClockwise();

private:
	glm::vec2 _center;
	float _radiusExpansionRate;
	bool _clockwise;
	float _totalTime;
};