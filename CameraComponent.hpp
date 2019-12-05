/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#pragma once

#include "sre/Camera.hpp"
#include "glm/glm.hpp"
#include "Component.hpp"

class CameraComponent : public Component {
public:
	explicit CameraComponent(GameObject* gameObject);

	void update(float deltaTime) override;

	// Uncomment to implement a follow camera in the future
	// void setFollowObject(std::shared_ptr<GameObject> followObject, glm::vec2 offset);

	sre::Camera& getCamera();

private:
	sre::Camera camera;
	/* // Uncomment to implement a follow camera in the future
	std::shared_ptr<GameObject> followObject;
	glm::vec2 offset;
	*/ 
};