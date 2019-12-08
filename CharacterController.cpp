/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#include <SDL_events.h>
#include <iostream>
#include "CharacterController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "LightOfTheMoon.hpp"
#include "SpriteComponent.hpp"
#include "AudioLocator.hpp"

CharacterController::CharacterController(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->getComponent<PhysicsComponent>();

    animatorComponent = gameObject->getComponent<AnimatorComponent>();

	characterHealth = gameObject->getComponent<HealthComponent>();
}

void CharacterController::setAnimations(std::shared_ptr<Animation> idle_right_anim,
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

bool CharacterController::onKey(SDL_Event &event) {
        switch (event.key.keysym.sym){
            case SDLK_LEFT:
            {
                left = event.type == SDL_KEYDOWN;
				break;
            }
            case SDLK_RIGHT:
            {
                right = event.type == SDL_KEYDOWN;
				break;
            }
			case SDLK_UP:
			{
				up = event.type == SDL_KEYDOWN;
				break;
			}
			case SDLK_DOWN:
			{
				down = event.type == SDL_KEYDOWN;
				break;
			}
			case SDLK_SPACE:
				dash = event.type == SDL_KEYDOWN;
				break;
            break;
        }

    return false;
}

void CharacterController::onMouse(SDL_Event &event) {
	direction.x = event.motion.x - LightOfTheMoon::windowSize.x / 2;
	direction.y = event.motion.y - LightOfTheMoon::windowSize.y / 2;
	direction = glm::normalize(direction);

	
	//std::cout << "Direction: (" << direction.x << ", " << direction.y << ")" << std::endl;

	//TODO SHOOT BULLETS towards direction (there's a field called direction)
}

void CharacterController::update(float deltaTime) {

	if (!characterHealth->isAlive()) {
		//TODO DEATH
		std::cout << "DEAD" << std::endl;
		return;
	}

	//TODO Remove
	if (debug_show_life) {

		std::cout << "Player Health: " << characterHealth->getCurrentHealth() << "/" << characterHealth->getMaxHealth() << std::endl;
		debug_show_life = false;
	}


	/*Get input axis-xy and normalize it*/
    characterPhysics->fixRotation();
    glm::vec2 movement{0,0};
    if (left){
        movement.x -= 1;
    }
    if (right){
        movement.x += 1;
    }
	if (up) {
		movement.y += 1;
	}
	if (down) {
		movement.y -= 1;
	}
	if (glm::length(movement) > 0.1f)
		movement = glm::normalize(movement);
	
	if (dashTimer > 0) {
		//During dash
		dashTimer -= deltaTime;
	}
	else {
		if (dash) {
			//Prepare to dash
			dash = false;
			characterPhysics->setLinearVelocity(dashSpeed*movement);
			dashTimer = dashDuration;
		}
		else {
			//Standard movement
			glm::vec2 speed = maxSpeed * movement;
			characterPhysics->setLinearVelocity(speed);
		}
	}
	
	updateAnimation(deltaTime);
}

void CharacterController::onCollisionStart(PhysicsComponent *comp) {
	debug_show_life = true; //TODO remove
}

void CharacterController::onCollisionEnd(PhysicsComponent *comp) {

}

void CharacterController::updateAnimation(float deltaTime) {
	//TODO Shooting animation

	float angle = glm::atan(-direction.y, direction.x);
	float angle_deg = glm::degrees(angle);
	
	if (angle_deg < -157.5f) {
		animatorComponent->setAnimation(idle_left_anim, false);
	}
	else if (angle_deg < -112.5f) {
		animatorComponent->setAnimation(idle_down_left_anim, false);
	}
	else if (angle_deg < -67.5f) {
		animatorComponent->setAnimation(idle_down_anim, false);
	}
	else if (angle_deg < -22.5f) {
		animatorComponent->setAnimation(idle_down_right_anim, false);
	}
	else if (angle_deg < 22.5f) {
		animatorComponent->setAnimation(idle_right_anim, false);
	}
	else if (angle_deg < 67.5f) {
		animatorComponent->setAnimation(idle_top_right_anim, false);
	}
	else if (angle_deg < 112.5f) {
		animatorComponent->setAnimation(idle_top_anim, false);
	}
	else if (angle_deg < 157.5f) {
		animatorComponent->setAnimation(idle_top_left_anim, false);
	}
	else {
		animatorComponent->setAnimation(idle_left_anim, false);
	}
}


