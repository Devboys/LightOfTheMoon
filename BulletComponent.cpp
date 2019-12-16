/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Deactivate gameObject of bullet on despawn
 * + Alberto Giudice [15/12/19] - Added logic to disappear on wall collision
 * + Alberto Giudice [14/12/19] - Logic implementation
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "BulletComponent.hpp"
#include "CharacterController.hpp"
#include "HealthComponent.hpp"
#include "LightOfTheMoon.hpp"
#include "PhysicsComponent.hpp"
#include "BossComponent.hpp"

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
		if (comp->getGameObject()->getComponent<BossComponent>() != nullptr) {
			std::shared_ptr<HealthComponent> bossHealth = comp->getGameObject()->getComponent<HealthComponent>();
			if (bossHealth != nullptr) {
				bossHealth->removeHealth(_damageAmount);
			}
			targetHit = true;
		}
	}
	else if (_type == BulletType::BossBullet) {
		if (comp->getGameObject()->getComponent<CharacterController>() != nullptr) {
			std::shared_ptr<HealthComponent> playerHealth = comp->getGameObject()->getComponent<HealthComponent>();
			if (playerHealth != nullptr) {
				playerHealth->removeHealth(_damageAmount);
			}
			targetHit = true;
		}
	}

	if (comp->getGameObject()->name == "Wall")
		targetHit = true;

	if (targetHit) {
		_inUse = false;
		gameObject->setActive(false);
		LightOfTheMoon::getInstance()->destroyGameObject(this->gameObject); // this doesn't destroy the object, it just goes back in the bullet pool
	}
}