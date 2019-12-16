/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice 15/12/19] - Implemented shooting through pool
 * + Alberto Giudice [15/12/19] - Added second sensor collider
 * + Francesco Frassineti [14/12/19] - Implemented shooting
 * + Francesco Frassineti [07/12/19] - Implemented animation and movement
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#define Y_SCALE_FACTOR 0.86602540378f //sqrt(3)/2

#include <SDL_events.h>
#include "CharacterController.hpp"
#include "GameObject.hpp"
#include "PhysicsComponent.hpp"
#include "LightOfTheMoon.hpp"
#include "SpriteComponent.hpp"
#include "AudioLocator.hpp"
#include "BulletComponent.hpp"
#include "MovementLinearComponent.hpp"

CharacterController::CharacterController(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->getComponent<PhysicsComponent>();

    animatorComponent = gameObject->getComponent<AnimatorComponent>();

	characterHealth = gameObject->getComponent<HealthComponent>();


	auto spriteAtlas = LightOfTheMoon::getInstance()->getSpriteAtlas();
	linearBulletSprites.push_back(spriteAtlas->get("bullet-cowboy-1.png"));
	linearBulletSprites.push_back(spriteAtlas->get("bullet-cowboy-2.png"));
	for (auto& s : linearBulletSprites) { s.setScale({ 0.0003f, 0.0003f }); }

	bulletPool = new BulletPool();
	bulletPool->createLinearPool();
}

CharacterController::~CharacterController() {
	delete bulletPool;
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
            case SDLK_a:
            {
                left = event.type == SDL_KEYDOWN;
				break;
            }
            case SDLK_d:
            {
                right = event.type == SDL_KEYDOWN;
				break;
            }
			case SDLK_w:
			{
				up = event.type == SDL_KEYDOWN;
				break;
			}
			case SDLK_s:
			{
				down = event.type == SDL_KEYDOWN;
				break;
			}
			case SDLK_SPACE:
			{
				dash = event.type == SDL_KEYDOWN;
				break;
			}
            break;
        }

    return false;
}

void CharacterController::onMouse(SDL_Event &event) {
	mouseX = event.motion.x - LightOfTheMoon::windowSize.x / 2;
	mouseY = event.motion.y - LightOfTheMoon::windowSize.y / 2;
	mouseY /= -(LightOfTheMoon::windowSize.y / 2); //They are both divided by windowsSize.x for a reason, it's not a bug
	mouseX /= (LightOfTheMoon::windowSize.y / 2);//They are both divided by windowsSize.x for a reason, it's not a bug
	direction.x = mouseX - gameObject->getPosition().x;
	direction.y = mouseY - gameObject->getPosition().y;
	direction = glm::normalize(direction);

	if (event.button.button == SDL_BUTTON_LEFT) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			shoot = true;
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			shoot = false;
		}
	}
}

void CharacterController::update(float deltaTime) {

	if (!characterHealth->isAlive()) {
		LightOfTheMoon::instance->requestChangeState(GameState::GameOver);
		return;
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
			AudioLocator::getService()->playOneshot("Assets/Sounds/DashSound.wav");
			characterPhysics->setLinearVelocity(dashSpeed*movement);
			dashTimer = dashDuration;
		}
		else {
			//Standard movement
			glm::vec2 speed = maxSpeed * movement;
			characterPhysics->setLinearVelocity(speed);
		}
	}
	
	if (shoot && shootingTimer <= 0) {//If ready to shoot
		// calculate the angle to shoot the bullet based on mouse position relative to character
		glm::vec2 direction = glm::vec2(mouseX, mouseY) - gameObject->getPosition();
		float directionAngleDeg = std::atan2f(direction.y, direction.x) * 180.0f / M_PI;

		// spawn a linear bullet from the character's pool
		bulletPool->spawnPlayerLinearBullet(gameObject->getPosition(), linearBulletSprites, 1, directionAngleDeg, bulletSpeed);
		// play bullet sound
		AudioLocator::getService()->playOneshot("Assets/Sounds/ShootSound.wav");

		shootingTimer = shootingCooldown; //Set cooldown
	}

	if (shootingTimer > 0)
		shootingTimer -= deltaTime;

	updateAnimation(deltaTime);
}

void CharacterController::updateAnimation(float deltaTime) {
 
	float angle = glm::atan((mouseY - gameObject->getPosition().y), mouseX - gameObject->getPosition().x);
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