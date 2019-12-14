/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
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
	BulletComponent(GameObject* gameObject);

	void initPlayerBullet(const int& damage);
	void initBossBullet(const int& damage);

	const bool inUse();

	void onCollisionStart(PhysicsComponent* comp) override;

	const int getDamage();
	void setDamage(const int& amount);

private:
	int _damageAmount;
	BulletType _type;
	bool _inUse;
};