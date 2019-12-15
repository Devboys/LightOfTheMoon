/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#include "Boss1Component.hpp"
#include "GameObject.hpp"
#include "LightOfTheMoon.hpp"

Boss1Component::Boss1Component(GameObject* gameObject) : Component(gameObject), _phase(Boss1Phase::First), _state(Boss1State::First_Idle) {
	bossHealth = gameObject->getComponent<HealthComponent>();
	bossAnimator = gameObject->getComponent<AnimatorComponent>();
	bossPhysics = gameObject->getComponent<PhysicsComponent>();

	direction = glm::vec2(-1, 0); //Start by facing west

	std::cout << "Boss Component initialized" << std::endl;
}

void Boss1Component::update(float deltaTime) {
	//Hierarchical State Machine (Phases are the higher level states, States the lower level ones)
	switch (_phase)
	{
	case Boss1Phase::First:
		updatePhase1(deltaTime);
		break;
	case Boss1Phase::Second:
		updatePhase2(deltaTime);
		break;
	case Boss1Phase::Third:
		updatePhase3(deltaTime);
		break;
	case Boss1Phase::Dead:
		std::cout << "Boss1 defeated" << std::endl;
		LightOfTheMoon::getInstance()->destroyGameObject(gameObject);
		break;
	default:
		break;
	}

	/*if (bossPhysics->getLinearVelocity().length() > 0.1f) {
		direction = glm::normalize(bossPhysics->getLinearVelocity());
	}*/

	updateAnimation(deltaTime);
}

void Boss1Component::updatePhase1(float deltaTime) {
	if (bossHealth->getCurrentHealth() < healthPercentageThresholds[0] * bossHealth->getMaxHealth()) {
		_phase = Boss1Phase::Second;
		_state = Boss1State::Second_Idle;
		bossHealth->print();
		std::cout << "Boss1: Transition to Phase 2" << std::endl;
	}

	switch (_state) {
	case Boss1State::First_Idle:
		//Something
		break;
	//case yada yada
	//...
	//...
	case Boss1State::First_StartMove:
		//Something
		break;
	}
}

void Boss1Component::updatePhase2(float deltaTime) {
	if (bossHealth->getCurrentHealth() < healthPercentageThresholds[1] * bossHealth->getMaxHealth()) {
		_phase = Boss1Phase::Third;
		_state = Boss1State::Third_Idle;
		bossHealth->print();
		std::cout << "Boss1: Transition to Phase 3" << std::endl;
	}

	switch (_state) {
	case Boss1State::Second_Idle:
		//Something
		break;
		//case yada yada
		//...
		//...
	case Boss1State::Second_Shoot:
		//Something
		break;
	}
}

void Boss1Component::updatePhase3(float deltaTime) {
	if (!bossHealth->isAlive()) {
		bossHealth->print();
		_phase = Boss1Phase::Dead;
		_state = Boss1State::Dead;
	}

	switch (_state) {
	case Boss1State::Third_Idle:
		//Something
		break;
		//case yada yada
		//...
		//...
	case Boss1State::Third_Shoot:
		//Something
		break;
	}
}

void Boss1Component::setAnimations(std::shared_ptr<Animation> idle_right_anim,
	std::shared_ptr<Animation> idle_top_right_anim,
	std::shared_ptr<Animation> idle_top_anim,
	std::shared_ptr<Animation> idle_top_left_anim,
	std::shared_ptr<Animation> idle_left_anim,
	std::shared_ptr<Animation> idle_down_left_anim,
	std::shared_ptr<Animation> idle_down_anim,
	std::shared_ptr<Animation> idle_down_right_anim
) {
	this->idle_right_anim = idle_right_anim;
	this->idle_top_right_anim = idle_top_right_anim;
	this->idle_top_anim = idle_top_anim;
	this->idle_top_left_anim = idle_top_left_anim;
	this->idle_left_anim = idle_left_anim;
	this->idle_down_left_anim = idle_down_left_anim;
	this->idle_down_anim = idle_down_anim;
	this->idle_down_right_anim = idle_down_right_anim;
}

void Boss1Component::updateAnimation(float deltaTime) {

	float angle = glm::atan(direction.y, direction.x);
	float angle_deg = glm::degrees(angle);

	if (angle_deg < -157.5f) {
		bossAnimator->setAnimation(idle_left_anim, false);
	}
	else if (angle_deg < -112.5f) {
		bossAnimator->setAnimation(idle_down_left_anim, false);
	}
	else if (angle_deg < -67.5f) {
		bossAnimator->setAnimation(idle_down_anim, false);
	}
	else if (angle_deg < -22.5f) {
		bossAnimator->setAnimation(idle_down_right_anim, false);
	}
	else if (angle_deg < 22.5f) {
		bossAnimator->setAnimation(idle_right_anim, false);
	}
	else if (angle_deg < 67.5f) {
		bossAnimator->setAnimation(idle_top_right_anim, false);
	}
	else if (angle_deg < 112.5f) {
		bossAnimator->setAnimation(idle_top_anim, false);
	}
	else if (angle_deg < 157.5f) {
		bossAnimator->setAnimation(idle_top_left_anim, false);
	}
	else {
		bossAnimator->setAnimation(idle_left_anim, false);
	}
}