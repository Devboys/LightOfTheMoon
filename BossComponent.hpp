/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Creation and implementation
 */

#pragma once

#include "Component.hpp"
#include "HealthComponent.hpp"
#include "AnimatorComponent.hpp"
#include "BulletPool.hpp"

 /*High level states*/
enum class BossPhase {
	First,
	Second,
	Third,
	Dead
};

/*Low level*/
enum class BossState {
	Idle,
	FirstShotPattern,
	SecondShotPattern,
	ThirdShotPattern
};

class BossComponent : public Component {
public:
	explicit BossComponent(GameObject* gameObject);
	~BossComponent();

	virtual void updateAnimation(const float& deltaTime) = 0;
	virtual void update(float deltaTime) override;

protected:
	BossPhase _phase;
	BossState _state;
	
	std::shared_ptr<HealthComponent> bossHealth;
	std::shared_ptr<AnimatorComponent> bossAnimator;

	BulletPool* bulletPool;

	// parameters for shooting, same for each boss
	float timeElapsedCurrentPhase = 0.0f;
	float shootingTimer = 0.0f;

	// cooldowns for each pattern of bullets used during a certain phase
	float shootingCooldownFirstPattern;
	float shootingCooldownSecondPattern;
	float shootingCooldownThirdPattern;
};