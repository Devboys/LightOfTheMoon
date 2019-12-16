/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Restricted usage of BulletComponent to BulletPool only
 * + Alberto Giudice [13/12/19] - Structure implementation
 * + Alberto Giudice [12/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"

enum class BulletType {
	PlayerBullet,
	BossBullet
};

class BulletComponent : public Component {
public:
	void initPlayerBullet(const int& damage);
	void initBossBullet(const int& damage);

	const bool inUse();

	void onCollisionStart(PhysicsComponent* comp) override;

	const int getDamage();
	void setDamage(const int& amount);

private:
	BulletComponent(GameObject* gameObject); // only the bullet pool can create new bullets, more safety
	int _damageAmount;
	BulletType _type;
	bool _inUse;

	friend class BulletPool;
	friend class GameObject;
};