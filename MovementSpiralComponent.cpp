/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - First implementation of methods
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#include "MovementSpiralComponent.hpp"

MovementSpiralComponent::MovementSpiralComponent(GameObject* gameObject) : Component(gameObject) {}

void MovementSpiralComponent::initParameters(const glm::vec2 center, const float& radiusExpansionRate, const bool& clockwise) {
	_center = center;
	_radiusExpansionRate = radiusExpansionRate;
	_clockwise = clockwise;
	_totalTime = 0.0f;
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

}