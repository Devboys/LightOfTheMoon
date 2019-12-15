/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Physic and rendering activation only on bullet spawn
 * + Alberto Giudice [15/12/19] - Proper implementation for every bullet type with 100% component reuse
 * + Alberto Giudice [15/12/19] - First Implementation of the three pools creation and the linear player bullet spawn
 * + Alberto Giudice [15/12/19] - Basic creation
 */

#pragma once

#include <iostream>
#include "BulletPool.hpp"
#include "BulletComponent.hpp"
#include "PhysicsComponent.hpp"
#include "AnimatorComponent.hpp"
#include "MovementLinearComponent.hpp"
#include "MovementSpiralComponent.hpp"
#include "MovementWaveComponent.hpp"
#include "LightOfTheMoon.hpp"


BulletPool* BulletPool::createLinearPool() {
	for (int i = 0; i < POOL_SIZE; i++) {
		auto obj = std::shared_ptr<GameObject>(new GameObject());
		obj->name = "LinearBullet";
		obj->setPosition({ 2000, 2000 });
		auto phys = obj->addComponent<PhysicsComponent>();
		phys->initCircle(b2_dynamicBody, 1.0f, { obj->getPosition().x / LightOfTheMoon::physicsScale, obj->getPosition().y / LightOfTheMoon::physicsScale }, 1, false);
		phys->fixRotation();
		phys->setSensor(true);
		phys->setBullet(true);
		phys->setAutoUpdate(false);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementLinearComponent>();

		linearBullets[i] = obj;
	}
	return this;
}

BulletPool* BulletPool::createWavePool() {
	for (int i = 0; i < POOL_SIZE; i++) {
		auto obj = std::shared_ptr<GameObject>(new GameObject());
		obj->name = "WaveBullet";
		obj->setPosition({ 2000, 2000 });
		auto phys = obj->addComponent<PhysicsComponent>();
		phys->initCircle(b2_dynamicBody, 1.0f, { obj->getPosition().x / LightOfTheMoon::physicsScale, obj->getPosition().y / LightOfTheMoon::physicsScale }, 1, false);
		phys->fixRotation();
		phys->setSensor(true);
		phys->setBullet(true);
		phys->setAutoUpdate(false);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementWaveComponent>();

		waveBullets[i] = obj;
	}
	return this;
}

BulletPool* BulletPool::createSpiralPool() {
	for (int i = 0; i < POOL_SIZE; i++) {
		auto obj = std::shared_ptr<GameObject>(new GameObject());
		obj->name = "SpiralBullet";
		obj->setPosition({ 2000, 2000 });
		auto phys = obj->addComponent<PhysicsComponent>();
		phys->initCircle(b2_dynamicBody, 1.0f, { obj->getPosition().x / LightOfTheMoon::physicsScale, obj->getPosition().y / LightOfTheMoon::physicsScale }, 1, false);
		phys->fixRotation();
		phys->setSensor(true);
		phys->setBullet(true);
		phys->setAutoUpdate(false);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementSpiralComponent>();

		spiralBullets[i] = obj;
	}
	return this;
}



void BulletPool::spawnPlayerLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true);
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initPlayerBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			assert(movement != nullptr);
			movement->initParameters(directionAngleDeg, velocity);

			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}


void BulletPool::spawnBossLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true);
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			assert(movement != nullptr);
			movement->initParameters(directionAngleDeg, velocity);

			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
} 


void BulletPool::spawnBossWaveBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& directionAngleDeg, const float& velocity, const float& amplitude, const float& frequency) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = waveBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = waveBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true);
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementWaveComponent> movement = bullet->getComponent<MovementWaveComponent>();
			assert(movement != nullptr);
			movement->initParameters(position, directionAngleDeg, velocity, amplitude, frequency);

			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}

void BulletPool::spawnBossSpiralBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = spiralBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = spiralBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true);
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementSpiralComponent> movement = bullet->getComponent<MovementSpiralComponent>();
			assert(movement != nullptr);
			movement->initParameters(position, minVelocity, maxVelocity, radiusExpansionRate, clockwise);

			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}