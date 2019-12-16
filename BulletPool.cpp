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

#include "BulletPool.hpp"
#include "BulletComponent.hpp"
#include "PhysicsComponent.hpp"
#include "AnimatorComponent.hpp"
#include "MovementLinearComponent.hpp"
#include "MovementSpiralComponent.hpp"
#include "MovementWaveComponent.hpp"
#include "LightOfTheMoon.hpp"

 // Creates a bullet pool of linear bullets of size POOL_SIZE
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
		phys->setAutoUpdate(true);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementLinearComponent>();

		linearBullets[i] = obj;
	}
	return this;
}

// Creates a bullet pool of wave bullets of size POOL_SIZE
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
		phys->setAutoUpdate(true);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementWaveComponent>();

		waveBullets[i] = obj;
	}
	return this;
}

// Creates a bullet pool of spiral bullets of size POOL_SIZE
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
		phys->setAutoUpdate(true);
		phys->setActive(false);
		auto bc = obj->addComponent<BulletComponent>();
		auto animator = obj->addComponent<AnimatorComponent>();
		auto movement = obj->addComponent<MovementSpiralComponent>();

		spiralBullets[i] = obj;
	}
	return this;
}


/* Spawns a linear bullet in the scene of type PlayerBullet
 *
 * @param position : The position the bullet will spawn in
 * @param sprites : Vector of sprites that should animate the bullet spawned
 * @param damage : Amount of damage the bullet should do when hitting a enemy
 * @param directionAngleDeg : direction the bullet will travel to, in degrees
 * @param velocity : constant speed the bullet should have in its linear trajectory
 */
void BulletPool::spawnPlayerLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true); // render this object

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true); // reactivate the box2d body to handle collisions again
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initPlayerBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			assert(movement != nullptr);
			movement->initParameters(directionAngleDeg, velocity);

			// add this bullet to the scene and use it
			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}

/* Spawns a linear bullet in the scene of type BossBullet
 *
 * @param position : The position the bullet will spawn in
 * @param sprites : Vector of sprites that should animate the bullet spawned
 * @param damage : Amount of damage the bullet should do when hitting the player
 * @param directionAngleDeg : direction the bullet will travel to, in degrees
 * @param velocity : constant speed the bullet should have in its linear trajectory
 */
void BulletPool::spawnBossLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
										const int& damage, const float& directionAngleDeg, const float& velocity) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = linearBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = linearBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true); // render this object

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true); // reactivate the box2d body to handle collisions again
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementLinearComponent> movement = bullet->getComponent<MovementLinearComponent>();
			assert(movement != nullptr);
			movement->initParameters(directionAngleDeg, velocity);

			// add this bullet to the scene and use it
			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
} 

/* Spawns a wave bullet in the scene of type BossBullet
 *
 * @param position : The position the bullet will spawn in
 * @param sprites : Vector of sprites that should animate the bullet spawned
 * @param damage : Amount of damage the bullet should do when hitting the player
 * @param directionAngleDeg : direction the bullet will travel to, in degrees
 * @param velocity : constant speed the bullet should travel at in the above mentioned direction
 * @param amplitude : height of the peaks in the sin wave generated
 * @param frequency : number of peaks over time the wave will make
 */
void BulletPool::spawnBossWaveBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& directionAngleDeg, const float& velocity, const float& amplitude, const float& frequency) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = waveBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = waveBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true); // render this object

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true); // reactivate the box2d body to handle collisions again
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementWaveComponent> movement = bullet->getComponent<MovementWaveComponent>();
			assert(movement != nullptr);
			movement->initParameters(position, directionAngleDeg, velocity, amplitude, frequency);

			// add this bullet to the scene and use it
			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}

/* Spawns a spiral bullet in the scene of type BossBullet
 *
 * @param position : The position the bullet will spawn in
 * @param sprites : Vector of sprites that should animate the bullet spawned
 * @param damage : Amount of damage the bullet should do when hitting the player
 * @param minVelocity : minimum angular speed the bullet will reach over time
 * @param maxVelocity : initial angular speed when the bullet is spawned. Will decrease over time
 * @param radiusExpansionRate : rate at which the radius of the spiral will increase over time
 * @param clockwise : true = clockwise rotation, false = counterclockwise rotation
 */
void BulletPool::spawnBossSpiralBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
										const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise) {
	bool spawned = false;
	for (int i = 0; !spawned && i < POOL_SIZE; i++) {
		std::shared_ptr<BulletComponent> bc = spiralBullets[i]->getComponent<BulletComponent>();
		assert(bc != nullptr);
		if (!bc->inUse()) {
			std::shared_ptr<GameObject> bullet = spiralBullets[i];
			bullet->setPosition(position);
			bullet->setActive(true); // render this object

			std::shared_ptr<PhysicsComponent> phys = bullet->getComponent<PhysicsComponent>();
			assert(phys != nullptr);
			phys->setPositionAndRotation({ bullet->getPosition().x / LightOfTheMoon::getInstance()->physicsScale,
									bullet->getPosition().y / LightOfTheMoon::getInstance()->physicsScale }, 0.0f);
			phys->setActive(true); // reactivate the box2d body to handle collisions again
			LightOfTheMoon::getInstance()->registerPhysicsComponent(phys.get());

			bc->initBossBullet(damage);

			std::shared_ptr<AnimatorComponent> animator = bullet->getComponent<AnimatorComponent>();
			assert(animator != nullptr);
			std::shared_ptr<Animation> animation = std::make_shared<Animation>(sprites, 1, true);
			animator->setAnimation(animation, true);

			std::shared_ptr<MovementSpiralComponent> movement = bullet->getComponent<MovementSpiralComponent>();
			assert(movement != nullptr);
			movement->initParameters(position, minVelocity, maxVelocity, radiusExpansionRate, clockwise);

			// add this bullet to the scene and use it
			LightOfTheMoon::getInstance()->addGameObject(bullet);

			spawned = true;
		}
	}
}