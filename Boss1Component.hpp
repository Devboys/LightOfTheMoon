/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Support for different sprites in different phases
 * + Alberto Giudice [15/12/19] - Changed the states options
 * + Francesco Frassineti [15/12/19] - Structure for the boss class created
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#pragma once

#include "BossComponent.hpp"
#include "PhysicsComponent.hpp"

class Boss1Component : public BossComponent {
public:
	explicit Boss1Component(GameObject* gameObject);

	void updateAnimation(const float& deltaTime) override;
	void update(float deltaTime) override;

	void setPlayer(std::shared_ptr<GameObject> player);//Set's the reference to the player, it's needed for the boss to keep looking towards the player

	void setAnimations(std::shared_ptr<Animation> idle_right_anim[3],
		std::shared_ptr<Animation> idle_top_right_anim[3],
		std::shared_ptr<Animation> idle_top_anim[3],
		std::shared_ptr<Animation> idle_top_left_anim[3],
		std::shared_ptr<Animation> idle_left_anim[3],
		std::shared_ptr<Animation> idle_down_left_anim[3],
		std::shared_ptr<Animation> idle_down_anim[3],
		std::shared_ptr<Animation> idle_down_right_anim[3]
	);

private:
	void updatePhase1(const float& deltaTime);
	void updatePhase2(const float& deltaTime);
	void updatePhase3(const float& deltaTime);

	std::shared_ptr<GameObject> player;

	std::shared_ptr<Animation> idle_right_anim[3];
	std::shared_ptr<Animation> idle_top_right_anim[3];
	std::shared_ptr<Animation> idle_top_anim[3];
	std::shared_ptr<Animation> idle_top_left_anim[3];
	std::shared_ptr<Animation> idle_left_anim[3];
	std::shared_ptr<Animation> idle_down_left_anim[3];
	std::shared_ptr<Animation> idle_down_anim[3];
	std::shared_ptr<Animation> idle_down_right_anim[3];

	glm::vec2 direction;/*The direction the boss is currently facing*/

	std::vector<sre::Sprite> linearBulletSprites;
	std::vector<sre::Sprite> waveBulletSprites;
	std::vector<sre::Sprite> spiralBulletSprites;

	float healthPercentageThresholds[2]{ 2.0f/3.0f, 1 / 3.0f };//For every phase, if the health percentage goes below the threshold switch to the next phase
};