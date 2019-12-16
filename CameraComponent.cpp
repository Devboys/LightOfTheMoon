/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include "CameraComponent.hpp"

using namespace glm;

CameraComponent::CameraComponent(GameObject* gameObject)
	: Component(gameObject){
}

sre::Camera& CameraComponent::getCamera() {
	return camera;
}

void CameraComponent::update(float deltaTime) {

}