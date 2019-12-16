/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Added a transform reset function for the bullet pool
 * + Alberto Giudice [15/12/19] - Added a method to create a second fixture as sensor
 * + Alberto Giudice [14/12/19] - Improved accuracy of moveTo on circular motions
 * + Francesco Frassineti [11/12/19] - Addeded edge creation
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#include <Box2D/Box2D.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include "PhysicsComponent.hpp"
#include "LightOfTheMoon.hpp"

PhysicsComponent::PhysicsComponent(GameObject* gameObject)
	: Component(gameObject)
{
	world = LightOfTheMoon::getInstance()->world;
}

PhysicsComponent::~PhysicsComponent() {
	delete polygon;
	delete circle;
	delete edge;

	// Check if the destructor is called due to the game being closed, if so the physicsComponentLookup is already cleared
	if (LightOfTheMoon::getInstance()->physicsComponentLookup.size() != 0) {
		LightOfTheMoon::getInstance()->deregisterPhysicsComponent(this);
		if (body != nullptr && fixture != nullptr) {
			body->DestroyFixture(fixture);
			fixture = nullptr;
			if (secondFixture != nullptr) {
				body->DestroyFixture(secondFixture);
				secondFixture = nullptr;
			}
		}
		if (world != nullptr && body != nullptr) {
			world->DestroyBody(body);
			body = nullptr;
		}
	}
}

void PhysicsComponent::addImpulse(glm::vec2 force) {
	b2Vec2 iForceV{ force.x,force.y };
	body->ApplyLinearImpulse(iForceV, body->GetWorldCenter(), true);
}

void PhysicsComponent::addForce(glm::vec2 force) {
	b2Vec2 forceV{ force.x,force.y };
	body->ApplyForce(forceV, body->GetWorldCenter(), true);
}

glm::vec2 PhysicsComponent::getLinearVelocity() {
	b2Vec2 v = body->GetLinearVelocity();
	return { v.x,v.y };
}

void PhysicsComponent::setLinearVelocity(glm::vec2 velocity) {
	b2Vec2 v{ velocity.x, velocity.y };
	if (velocity != glm::vec2(0, 0)) {
		body->SetAwake(true);
	}
	body->SetLinearVelocity(v);
}

void PhysicsComponent::initCircle(b2BodyType type, float radius, glm::vec2 center, float density, const bool& registerComponent) {
	assert(body == nullptr);
	autoUpdate = type != b2_staticBody;
	// do init
	shapeType = b2Shape::Type::e_circle;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	circle = new b2CircleShape();
	circle->m_radius = radius;
	b2FixtureDef fxD;
	fxD.userData = (void*)"Circle";
	fxD.shape = circle;
	fxD.density = density;
	fixture = body->CreateFixture(&fxD);
	spriteOffset = { 0.0f, 0.0f };
	if(registerComponent)
		LightOfTheMoon::getInstance()->registerPhysicsComponent(this);
}

void PhysicsComponent::initBox(b2BodyType type, glm::vec2 size, glm::vec2 center, float density, const bool& registerComponent) {
	assert(body == nullptr);
	autoUpdate = type != b2_staticBody;
	// do init
	shapeType = b2Shape::Type::e_polygon;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	polygon = new b2PolygonShape();
	polygon->SetAsBox(size.x, size.y, { 0,0 }, 0);
	b2FixtureDef fxD;
	fxD.userData = (void*)"Box";
	fxD.shape = polygon;
	fxD.density = density;
	fixture = body->CreateFixture(&fxD);
	spriteOffset = { 0.0f, 0.0f };

	if(registerComponent)
		LightOfTheMoon::getInstance()->registerPhysicsComponent(this);
}

void PhysicsComponent::initEdge(b2BodyType type, glm::vec2 center, glm::vec2 v1, glm::vec2 v2, const bool& registerComponent){
	assert(body == nullptr);
	autoUpdate = type != b2_staticBody;
	// do init
	shapeType = b2Shape::Type::e_edge;
	b2BodyDef bd;
	bd.type = type;
	rbType = type;
	bd.position = b2Vec2(center.x, center.y);
	body = world->CreateBody(&bd);
	edge = new b2EdgeShape();
	edge->Set(b2Vec2(v1.x, v1.y), b2Vec2(v2.x, v2.y));
	b2FixtureDef fxD;
	fxD.userData = (void*)"Edge";
	fxD.shape = edge;
	//fxD.density = density;
	fixture = body->CreateFixture(&fxD);
	spriteOffset = { 0.0f, 0.0f };

	if(registerComponent)
		LightOfTheMoon::getInstance()->registerPhysicsComponent(this);
}

void PhysicsComponent::initSensorBox(glm::vec2 size, glm::vec2 center, float density, glm::vec2 offset) {
	polygon = new b2PolygonShape();
	polygon->SetAsBox(size.x, size.y, { offset.x, offset.y }, 0);
	b2FixtureDef fxD;
	fxD.userData = (void*)"Box";
	fxD.shape = polygon;
	fxD.density = density;
	fxD.isSensor = true;
	secondFixture = body->CreateFixture(&fxD);
	spriteOffset = offset;
	LightOfTheMoon::getInstance()->registerSecondPhysicsComponent(this);
}

bool PhysicsComponent::isSensor() {
	return fixture->IsSensor();
}

void PhysicsComponent::setSensor(bool enabled) {
	fixture->SetSensor(enabled);
}

bool PhysicsComponent::isBullet() {
	return body->IsBullet();
}

void PhysicsComponent::setBullet(bool enabled) {
	body->SetBullet(enabled);
}

void PhysicsComponent::fixRotation() {
	fixture->GetBody()->SetFixedRotation(true);
}

void PhysicsComponent::moveTo(glm::vec2 pos) {
	glm::vec2 delta = pos - getPosition();

	setLinearVelocity(delta * (2 / LightOfTheMoon::timeStep));
}

glm::vec2 PhysicsComponent::getPosition() {
	return glm::vec2(body->GetPosition().x, body->GetPosition().y);
}

void PhysicsComponent::setPositionAndRotation(const glm::vec2& pos, const float& rot) {
	body->SetTransform({ pos.x, pos.y }, 0.0f);
}

bool PhysicsComponent::isAutoUpdate() const {
	return autoUpdate;
}

void PhysicsComponent::setAutoUpdate(bool autoUpdate) {
	PhysicsComponent::autoUpdate = autoUpdate;
}

b2Body* PhysicsComponent::getBody() {
	return body;
}

b2Fixture* PhysicsComponent::getFixture() {
	return fixture;
}

b2Fixture* PhysicsComponent::getSecondFixture() {
	return secondFixture;
}

const glm::vec2 PhysicsComponent::getSpriteOffset() {
	return spriteOffset;
}

void PhysicsComponent::setActive(const bool& active) {
	body->SetActive(active);
}