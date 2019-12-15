/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Added logic to disappear on wall collision
 * + Alberto Giudice [14/12/19] - Logic implementation
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include <iostream>
#include "BulletComponent.hpp"
#include "CharacterController.hpp"
#include "BossController.hpp"
#include "HealthComponent.hpp"
#include "LightOfTheMoon.hpp"
#include "PhysicsComponent.hpp"

BulletComponent::BulletComponent(GameObject* gameObject) : Component(gameObject), _inUse(false) {}

void BulletComponent::initPlayerBullet(const int& damage) {
	_inUse = true;
	_type = BulletType::PlayerBullet;
	_damageAmount = damage;
}

void BulletComponent::initBossBullet(const int& damage) {
	_inUse = true;
	_type = BulletType::BossBullet;
	_damageAmount = damage;
}

const bool BulletComponent::inUse() {
	return _inUse;
}

const int BulletComponent::getDamage() {
	return _damageAmount;
}

void BulletComponent::setDamage(const int& amount) {
	_damageAmount = amount;
}

void BulletComponent::onCollisionStart(PhysicsComponent* comp) {
	bool targetHit = false;
	if (_type == BulletType::PlayerBullet) {
		if (comp->getGameObject()->getComponent<BossController>() != nullptr) {
			std::shared_ptr<HealthComponent> bossHealth = comp->getGameObject()->getComponent<HealthComponent>();
			if (bossHealth != nullptr) {
				bossHealth->removeHealth(_damageAmount);
				std::cout << "CURR BOSS HEALTH: " << bossHealth->getCurrentHealth() << std::endl;
			}
			targetHit = true;
		}
	}
	else if (_type == BulletType::BossBullet) {
		if (comp->getGameObject()->getComponent<CharacterController>() != nullptr) {
			std::shared_ptr<HealthComponent> playerHealth = comp->getGameObject()->getComponent<HealthComponent>();
			if (playerHealth != nullptr) {
				playerHealth->removeHealth(_damageAmount);
				std::cout << "CURR PLAYER HEALTH: " << playerHealth->getCurrentHealth() << std::endl;
			}
			targetHit = true;
		}
	}

	if (comp->getGameObject()->name == "Wall")
		targetHit = true;

	if (targetHit) {
		//std::cout << "BULLET DESTROYED" << std::endl;
		_inUse = false;
		LightOfTheMoon::getInstance()->destroyGameObject(this->gameObject);
	}
	
	//std::cout << comp->getGameObject()->name << std::endl;
}