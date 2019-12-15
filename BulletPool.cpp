/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
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
		auto linearBulletObj = std::shared_ptr<GameObject>(new GameObject());
		linearBulletObj->name = "LinearBullet";

		auto phys = linearBulletObj->addComponent<PhysicsComponent>();
		auto bc = linearBulletObj->addComponent<BulletComponent>();
		auto animator = linearBulletObj->addComponent<AnimatorComponent>();
		auto movement = linearBulletObj->addComponent<MovementLinearComponent>();

		linearBullets[i] = linearBulletObj;
	}
	return this;
}

BulletPool* BulletPool::createWavePool() {
	for (int i = 0; i < POOL_SIZE; i++) {
		auto waveBulletObj = std::shared_ptr<GameObject>(new GameObject());
		waveBulletObj->name = "WaveBullet";

		auto phys = waveBulletObj->addComponent<PhysicsComponent>();
		auto bc = waveBulletObj->addComponent<BulletComponent>();
		auto animator = waveBulletObj->addComponent<AnimatorComponent>();
		auto movement = waveBulletObj->addComponent<MovementWaveComponent>();

		waveBullets[i] = waveBulletObj;
	}
	return this;
}

BulletPool* BulletPool::createSpiralPool() {
	for (int i = 0; i < POOL_SIZE; i++) {
		auto spiralWaveObj = std::shared_ptr<GameObject>(new GameObject());
		spiralWaveObj->name = "SpiralBullet";

		auto phys = spiralWaveObj->addComponent<PhysicsComponent>();
		auto bc = spiralWaveObj->addComponent<BulletComponent>();
		auto animator = spiralWaveObj->addComponent<AnimatorComponent>();
		auto movement = spiralWaveObj->addComponent<MovementLinearComponent>();

		spiralBullets[i] = spiralWaveObj;
	}
	return this;
}



void BulletPool::spawnPlayerLinearBullet(const glm::vec2& position,  std::vector<sre::Sprite> sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			if (phys != nullptr) {
				if (phys->getBody() == nullptr)
					phys->initCircle(b2_dynamicBody, 1.0f, { bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
										bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 1);
				else {
					phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
					LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());
				}
				phys->fixRotation();
				phys->setSensor(true);
				phys->setBullet(true);
			}

			bc->initPlayerBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			if (animator != nullptr) {
				for (auto& s : sprites) { s.setScale({ 0.0003f, 0.0003f }); }
				std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
				animator->setAnimation(animation, true);
			}

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			movement->initParameters(directionAngleDeg, velocity);

			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}

void BulletPool::spawnPlayerLinearBullet(const glm::vec2& position, std::vector<sre::Sprite> sprites,
										const int& damage, const glm::vec2 & direction, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::cout << "BULLET SPAWNED: " << i << std::endl;
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			if (phys != nullptr) {
				if (phys->getBody() == nullptr)
					phys->initCircle(b2_dynamicBody, 1.0f, { bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
										bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 1);
				else {
					phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
					LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());
				}
				phys->fixRotation();
				phys->setSensor(true);
				phys->setBullet(true);
			}

			bc->initPlayerBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			if (animator != nullptr) {
				for (auto& s : sprites) { s.setScale({ 0.0003f, 0.0003f }); }
				std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
				animator->setAnimation(animation, true);
			}

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			movement->initParameters(direction, velocity);

			spawned = true;
		}
	}
}



void BulletPool::spawnBossLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {

} 

void BulletPool::spawnBossLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const glm::vec2 & direction, const float& velocity) {

}

void BulletPool::spawnBossWaveBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& directionAngleDeg, const float& velocity, const float& amplitude, const float& frequency) {

}

void BulletPool::spawnBossSpiralBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise) {

}