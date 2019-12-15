/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#pragma once

#include <Box2D/Box2D.h>
#include "Component.hpp"
#include "AnimatorComponent.hpp"
#include "HealthComponent.hpp"
#include "sre/Sprite.hpp"
#include "BulletPool.hpp"

class CharacterController : public Component{
public:
    explicit CharacterController(GameObject *gameObject);
	~CharacterController();

    void setAnimations(std::shared_ptr<Animation> idle_right_anim,
		std::shared_ptr<Animation> idle_top_right_anim,
		std::shared_ptr<Animation> idle_top_anim,
		std::shared_ptr<Animation> idle_top_left_anim,
		std::shared_ptr<Animation> idle_left_anim,
		std::shared_ptr<Animation> idle_down_left_anim,
		std::shared_ptr<Animation> idle_down_anim,
		std::shared_ptr<Animation> idle_down_right_anim
		);

    void updateAnimation(float deltaTime);

    void update(float deltaTime) override;

    bool onKey(SDL_Event &event) override;

	void onMouse(SDL_Event &event) override;

private:
	void spawnPlayerBullet(glm::vec2& direction);

	std::shared_ptr<Animation> idle_right_anim;
	std::shared_ptr<Animation> idle_top_right_anim;
	std::shared_ptr<Animation> idle_top_anim;
	std::shared_ptr<Animation> idle_top_left_anim;
	std::shared_ptr<Animation> idle_left_anim;
	std::shared_ptr<Animation> idle_down_left_anim;
	std::shared_ptr<Animation> idle_down_anim;
	std::shared_ptr<Animation> idle_down_right_anim;

    std::shared_ptr<AnimatorComponent> animatorComponent;
    std::shared_ptr<PhysicsComponent> characterPhysics;
    std::shared_ptr<HealthComponent> characterHealth;

	BulletPool* bulletPool;

	glm::vec2 direction;
	float mouseX;
	float mouseY;

	float maxSpeed = 50.0f;

	float dashSpeed = 200.0f;
	float dashDuration = 0.1f;
	float dashTimer; //Keeps track of the time the character has beeen dashing. If dashTimer = 0 the character is not dashing

	float bulletSpeed = 50.0f;
	float bulletOffset = 0.03f;
	float shootingCooldown = 1.0 / 8.0;
	float shootingTimer = 0.0f;

    bool left = false;
    bool right = false;
	bool up = false;
	bool down = false;
	bool dash = false;
	bool shoot = false;
};
