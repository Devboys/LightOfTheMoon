/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "HealthComponent.hpp"
#include "AnimatorComponent.hpp"
#include "PhysicsComponent.hpp"

/*High level states*/
enum class Boss1Phase {
	First,
	Second,
	Third,
	Dead
};

/*Low level*/
enum class Boss1State {
	First_Idle,
	First_StartMove,
	First_Move,
	First_Shoot,
	Second_Idle,
	Second_StartMove,
	Second_Move,
	Second_Shoot,
	Third_Idle,
	Third_StartMove,
	Third_Move,
	Third_Shoot,
	Dead
};

class Boss1Component : public Component {

	

public:
	explicit Boss1Component(GameObject* gameObject);

	void updateAnimation(float deltaTime);
	void update(float deltaTime) override;

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
	void updatePhase1(float deltaTime);
	void updatePhase2(float deltaTime);
	void updatePhase3(float deltaTime);

	Boss1Phase _phase;
	Boss1State _state;
	std::shared_ptr<HealthComponent> bossHealth;
	std::shared_ptr<AnimatorComponent> bossAnimator;
	std::shared_ptr<PhysicsComponent> bossPhysics;

	std::shared_ptr<Animation> idle_right_anim;
	std::shared_ptr<Animation> idle_top_right_anim;
	std::shared_ptr<Animation> idle_top_anim;
	std::shared_ptr<Animation> idle_top_left_anim;
	std::shared_ptr<Animation> idle_left_anim;
	std::shared_ptr<Animation> idle_down_left_anim;
	std::shared_ptr<Animation> idle_down_anim;
	std::shared_ptr<Animation> idle_down_right_anim;

	glm::vec2 direction;

	float healthPercentageThresholds[2]{ 0.66, 0.33 };//For every phase, if the health percentage goes below the threshold switch to the next phase
};