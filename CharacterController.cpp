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
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "LightOfTheMoon.hpp"
#include "SpriteComponent.hpp"

CharacterController::CharacterController(GameObject *gameObject) : Component(gameObject) {
    characterPhysics = gameObject->getComponent<PhysicsComponent>();

    spriteComponent = gameObject->getComponent<SpriteComponent>();

	characterHealth = gameObject->getComponent<HealthComponent>();
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
	

	//TODO UPDATE ANIMATION

    //updateSprite(deltaTime);
}

void CharacterController::onCollisionStart(PhysicsComponent *comp) {
	debug_show_life = true; //TODO remove
}

void CharacterController::onCollisionEnd(PhysicsComponent *comp) {

}

void CharacterController::setSprites(sre::Sprite standing, sre::Sprite walk1, sre::Sprite walk2, sre::Sprite flyUp,
                                     sre::Sprite fly, sre::Sprite flyDown) {
    this->standing = standing;
    this->walk1 = walk1;
    this->walk2 = walk2;
    this->flyUp = flyUp;
    this->fly = fly;
    this->flyDown = flyDown;
}

//TO TRANSFORM INTO UPDATE ANIMATION
void CharacterController::updateSprite(float deltaTime) {
    /*auto velocity = characterPhysics->getLinearVelocity();

	animationTimer += animationSpeed * velocity.x * deltaTime;

	if (glm::length(velocity) <= 0.01) {
		spriteComponent->setSprite(this->standing);
	}
	else {
		if (isGrounded) {
			this->walk1.setFlip({ velocity.x < 0, false });
			this->walk2.setFlip({ velocity.x < 0, false });
			spriteComponent->setSprite((int)animationTimer % 2 == 0 ? this->walk1 : this->walk2);
		}
		else {
			if (glm::abs(velocity.y) <= midairVelocityThreshold) {
				//MID JUMP
				this->fly.setFlip({ velocity.x < 0, false });
				spriteComponent->setSprite(this->fly);
			}
			else if(velocity.y > 0) {
				//JUMP UP
				this->flyUp.setFlip({ velocity.x < 0, false });
				spriteComponent->setSprite(this->flyUp);
			}
			else {
				//FALLING
				this->flyDown.setFlip({ velocity.x < 0, false });
				spriteComponent->setSprite(this->flyDown);
			}
		}
	}
	*/
}


