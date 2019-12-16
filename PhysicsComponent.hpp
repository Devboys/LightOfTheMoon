/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Added method to deactivate physics simulation on bodies
 * + Alberto Giudice [15/12/19] - Second fixture support
 * + Alberto Giudice [14/12/19] - Added support for bullets (CCD)
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#pragma once

#include "Box2D/Common/b2Math.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Component.hpp"

class PhysicsComponent : public Component {
public:
	explicit PhysicsComponent(GameObject* gameObject);
	virtual ~PhysicsComponent();

	void initCircle(b2BodyType type, float radius, glm::vec2 center, float density, const bool& registerComponent = true);
	void initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density, const bool& registerComponent = true);
	void initEdge(b2BodyType type, glm::vec2 center, glm::vec2 v1, glm::vec2 v2, const bool& registerComponent = true);

	void initSensorBox(glm::vec2 size, glm::vec2 center, float density, glm::vec2 offset = { 0,0 });

	void addForce(glm::vec2 force);     // Force gradually affects the velocity over time
	void addImpulse(glm::vec2 force);   // Instantly affects velocity

	void setLinearVelocity(glm::vec2 velocity);
	glm::vec2 getLinearVelocity();

	bool isSensor();
	void setSensor(bool enabled);

	bool isBullet();
	void setBullet(bool enabled);

	void fixRotation();

	// Move to position in physics scale
	// Internally implemented by setting linearVelocity to delta value
	// This will make objects move until setLinearVelocity({0,0}) is called.
	void moveTo(glm::vec2 pos);

	// Get position in physics scale
	glm::vec2 getPosition();

	void setPositionAndRotation(const glm::vec2& pos, const float& rot);

	//  is updating sprite transform based on ridig body
	bool isAutoUpdate() const;
	//  update sprite transform based on ridig body
	void setAutoUpdate(bool autoUpdate);

	b2Body* getBody();
	b2Fixture* getFixture(); b2Fixture* getSecondFixture();

	const glm::vec2 getSpriteOffset();

	void setActive(const bool& active);

private:
	// auto update sprite position and rotation based on physics update
	bool autoUpdate = true;
	b2PolygonShape* polygon = nullptr;
	b2CircleShape* circle = nullptr;
	b2EdgeShape* edge = nullptr;
	b2Body* body = nullptr;
	b2Shape::Type shapeType;
	b2Fixture* fixture = nullptr;
	b2Fixture* secondFixture = nullptr;
	b2BodyType rbType;
	std::vector<PhysicsComponent*> collidingBodies;
	b2World* world = nullptr;
	glm::vec2 spriteOffset;
};