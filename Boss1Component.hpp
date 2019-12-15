/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Changed the states options
 * + Francesco Frassineti [15/12/19] - Structure for the boss class created
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "HealthComponent.hpp"
#include "AnimatorComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BulletPool.hpp"

/*High level states*/
enum class Boss1Phase {
	First,
	Second,
	Third,
	Dead
};

/*Low level*/
enum class Boss1State {
	Idle,
	FirstShotPattern,
	SecondShotPattern,
	ThirdShotPattern
};

class Boss1Component : public Component {
public:
	explicit Boss1Component(GameObject* gameObject);
	~Boss1Component();

	void updateAnimation(const float& deltaTime);
	void update(float deltaTime) override;

	void setPlayer(std::shared_ptr<GameObject> player);//Set's the reference to the player, it's needed for the boss to keep looking towards the player

	void setAnimations(std::shared_ptr<Animation> idle_right_anim,
		std::shared_ptr<Animation> idle_top_right_anim,
		std::shared_ptr<Animation> idle_top_anim,
		std::shared_ptr<Animation> idle_top_left_anim,
		std::shared_ptr<Animation> idle_left_anim,
		std::shared_ptr<Animation> idle_down_left_anim,
		std::shared_ptr<Animation> idle_down_anim,
		std::shared_ptr<Animation> idle_down_right_anim
	);

private:
	void updatePhase1(const float& deltaTime);
	void updatePhase2(const float& deltaTime);
	void updatePhase3(const float& deltaTime);

	Boss1Phase _phase;
	Boss1State _state;
	std::shared_ptr<HealthComponent> bossHealth;
	std::shared_ptr<AnimatorComponent> bossAnimator;

	std::shared_ptr<GameObject> player;

	std::shared_ptr<Animation> idle_right_anim;
	std::shared_ptr<Animation> idle_top_right_anim;
	std::shared_ptr<Animation> idle_top_anim;
	std::shared_ptr<Animation> idle_top_left_anim;
	std::shared_ptr<Animation> idle_left_anim;
	std::shared_ptr<Animation> idle_down_left_anim;
	std::shared_ptr<Animation> idle_down_anim;
	std::shared_ptr<Animation> idle_down_right_anim;

	glm::vec2 direction;/*The direction the boss is currently facing*/

	BulletPool* bulletPool;

	std::vector<sre::Sprite> linearBulletSprites;
	std::vector<sre::Sprite> waveBulletSprites;
	std::vector<sre::Sprite> spiralBulletSprites;

	float healthPercentageThresholds[2]{ 2.0f/3.0f, 1 / 3.0f };//For every phase, if the health percentage goes below the threshold switch to the next phase
};