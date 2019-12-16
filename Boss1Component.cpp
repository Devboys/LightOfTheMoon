/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Logic to handle sprite change on phase change
 * + Alberto Giudice [15/12/19] - Bullet spawning logic for all phases of the combat
 * + Alberto Giudice [15/12/19] - Initialized the bullet pools
 * + Francesco Frassineti [15/12/19] - Logic implementation for animations and states
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#include "Boss1Component.hpp"
#include "GameObject.hpp"
#include "LightOfTheMoon.hpp"
#include <stdlib.h>
#include <time.h>
#include "AudioLocator.hpp"

Boss1Component::Boss1Component(GameObject* gameObject) : BossComponent(gameObject) {
	bossHealth = gameObject->getComponent<HealthComponent>();
	bossAnimator = gameObject->getComponent<AnimatorComponent>();

	direction = glm::vec2(0, -1); // Start by facing south
	
	shootingCooldownFirstPattern = 0.4f;
	shootingCooldownSecondPattern = 0.6f;
	shootingCooldownThirdPattern = 0.0f;

	auto spriteAtlas = LightOfTheMoon::getInstance()->getSpriteAtlas();

	linearBulletSprites.push_back(spriteAtlas->get("bullet-enemy-1-1.png"));
	linearBulletSprites.push_back(spriteAtlas->get("bullet-enemy-1-2.png"));
	for (auto& s : linearBulletSprites) { s.setScale({ 0.0003f, 0.0003f }); }

	waveBulletSprites.push_back(spriteAtlas->get("bullet-enemy-2-1.png"));
	waveBulletSprites.push_back(spriteAtlas->get("bullet-enemy-2-2.png"));
	for (auto& s : waveBulletSprites) { s.setScale({ 0.0003f, 0.0003f }); }

	spiralBulletSprites.push_back(spriteAtlas->get("bullet-enemy-3-1.png"));
	spiralBulletSprites.push_back(spriteAtlas->get("bullet-enemy-3-2.png"));
	for (auto& s : spiralBulletSprites) { s.setScale({ 0.0003f, 0.0003f }); }

	bulletPool = new BulletPool();
	bulletPool->createLinearPool();
	bulletPool->createWavePool();
	bulletPool->createSpiralPool();

	srand(time(NULL));
}

void Boss1Component::update(float deltaTime) {
	//Hierarchical State Machine (Phases are the higher level states, States the lower level ones)
	switch (_phase)
	{
	case BossPhase::First:
		updatePhase1(deltaTime);
		break;
	case BossPhase::Second:
		updatePhase2(deltaTime);
		break;
	case BossPhase::Third:
		updatePhase3(deltaTime);
		break;
	case BossPhase::Dead:
		LightOfTheMoon::getInstance()->destroyGameObject(gameObject);
		LightOfTheMoon::getInstance()->requestChangeState(GameState::Win);
		break;
	default:
		break;
	}

	updateAnimation(deltaTime);
}

void Boss1Component::updatePhase1(const float& deltaTime) {
	/*ALL TRANSITIONS INSIDE IF STATEMENTS*/
	//If current health <= threshold -> go to next phase
	if (bossHealth->getCurrentHealth() <= healthPercentageThresholds[0] * bossHealth->getMaxHealth()) {
		_phase = BossPhase::Second;
		_state = BossState::Idle;

		// new cooldown time for phase 2
		shootingCooldownFirstPattern = 0.2f;
		shootingCooldownSecondPattern = 0.2f;
		shootingCooldownThirdPattern = 0.8f;

		timeElapsedCurrentPhase = 0.0f;
		shootingTimer = 0.0f;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();
	glm::vec2 direction = player->getPosition() - curPosition;
	float directionAngle = std::atan2f(direction.y, direction.x) * 180.0f / M_PI;

	bool bulletFired = false;
	/*LOGIC FOR THE CURRENT PHASE*/
	switch (_state) {
		case BossState::Idle:
			if (timeElapsedCurrentPhase > .5f)
				_state = BossState::FirstShotPattern;
			break;
		case BossState::FirstShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 30, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 30, 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownFirstPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 2.0f) > 1.0f)
				_state = BossState::SecondShotPattern;
			break;
		case BossState::SecondShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 15, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 15, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 45, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 45, 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownSecondPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 2.0f) < 1.0f)
				_state = BossState::FirstShotPattern;
			break;
		default:
			break;
	}
	if(bulletFired)
		AudioLocator::getService()->playOneshot("Assets/Sounds/BossShootSound.wav");

	shootingTimer -= deltaTime;
	timeElapsedCurrentPhase += deltaTime;
}

void Boss1Component::updatePhase2(const float& deltaTime) {
	if (bossHealth->getCurrentHealth() <= healthPercentageThresholds[1] * bossHealth->getMaxHealth()) {
		_phase = BossPhase::Third;
		_state = BossState::Idle;

		// set the cooldown times for phase 3
		shootingCooldownFirstPattern = 0.3f;
		shootingCooldownSecondPattern = 0.3f;
		shootingCooldownThirdPattern = 0.15f;

		timeElapsedCurrentPhase = 0.0f;
		shootingTimer = 0.0f;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();
	float offsetX = (float)(rand() % 101) / 100 - 0.5f;
	float offsetY = (float)(rand() % 101) / 100 - 0.5f;

	bool bulletFired = false;

	switch (_state) {
	case BossState::Idle:
		if (timeElapsedCurrentPhase > .5f)
			_state = BossState::FirstShotPattern;
		break;
	case BossState::FirstShotPattern:
		if (shootingTimer < 0.0f) {
			bulletPool->spawnBossSpiralBullet(curPosition + glm::vec2(offsetX, offsetY), spiralBulletSprites, 30, 40.0f, 150.0f, .05f, true);
			bulletFired = true;
			shootingTimer = shootingCooldownFirstPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 5.0f) > 1.0f)
			_state = BossState::SecondShotPattern;
		break;
	case BossState::SecondShotPattern:
		if (shootingTimer < 0.0f) {
			bulletPool->spawnBossSpiralBullet(curPosition + glm::vec2(offsetX, offsetY), spiralBulletSprites, 30, 40.0f, 150.0f, .05f, false);
			bulletFired = true;
			shootingTimer = shootingCooldownSecondPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 5.0f) > 2.0f)
			_state = BossState::ThirdShotPattern;
		break;
	case BossState::ThirdShotPattern:
		if (shootingTimer < 0.0f) {
			for (int angle = 0; angle < 360; angle += 15) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, angle + (rand() % 7 - 3), 50.0f);//Spawn radially
			}
			bulletFired = true;
			shootingTimer = shootingCooldownThirdPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 5.0f) < 1.0f)
			_state = BossState::FirstShotPattern;
		break;
	default:
		break;
	}

	if (bulletFired)
		AudioLocator::getService()->playOneshot("Assets/Sounds/BossShootSound.wav");

	shootingTimer -= deltaTime;
	timeElapsedCurrentPhase += deltaTime;
}

void Boss1Component::updatePhase3(const float& deltaTime) {
	if (!bossHealth->isAlive()) {
		_phase = BossPhase::Dead;
		_state = BossState::Idle;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();

	bool bulletFired = false;

	switch (_state) {
	case BossState::Idle:
		if (timeElapsedCurrentPhase > .5f)
			_state = BossState::FirstShotPattern;
		break;
	case BossState::FirstShotPattern:
		if (shootingTimer < 0.0f) {
			for (float angle = 22.5f; angle < 350.0f; angle += 45.0f) {
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, angle, .4f, .08f, 8.0f);
			}
			bulletFired = true;
			shootingTimer = shootingCooldownFirstPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 4.0f) > 1.0f)
			_state = BossState::SecondShotPattern;
		break;
	case BossState::SecondShotPattern:
		if (shootingTimer < 0.0f) {
			for (int angle = 0; angle < 360; angle += 45) {
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, angle, .4f, .08f, 8.0f);
			}
			bulletFired = true;
			shootingTimer = shootingCooldownFirstPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 4.0f) > 2.0f)
			_state = BossState::ThirdShotPattern;
		break;
	case BossState::ThirdShotPattern:
		if (shootingTimer < 0.0f) {
			for (int angle = 0; angle < 360; angle += 45) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, angle + (rand() % 21 - 10), 50.0f);
			}
			bulletFired = true;
			shootingTimer = shootingCooldownThirdPattern;
		}
		if (fmodf(timeElapsedCurrentPhase, 4.0f) < 1.0f)
			_state = BossState::FirstShotPattern;
		break;
	default:
		break;
	}

	if (bulletFired)
		AudioLocator::getService()->playOneshot("Assets/Sounds/BossShootSound.wav");

	shootingTimer -= deltaTime;
	timeElapsedCurrentPhase += deltaTime;
}

void Boss1Component::setAnimations(std::shared_ptr<Animation> idle_right_anim[3],
	std::shared_ptr<Animation> idle_top_right_anim[3],
	std::shared_ptr<Animation> idle_top_anim[3],
	std::shared_ptr<Animation> idle_top_left_anim[3],
	std::shared_ptr<Animation> idle_left_anim[3],
	std::shared_ptr<Animation> idle_down_left_anim[3],
	std::shared_ptr<Animation> idle_down_anim[3],
	std::shared_ptr<Animation> idle_down_right_anim[3]
) {
	this->idle_right_anim[0] = idle_right_anim[0];
	this->idle_right_anim[1] = idle_right_anim[1];
	this->idle_right_anim[2] = idle_right_anim[2];
	this->idle_top_right_anim[0] = idle_top_right_anim[0];
	this->idle_top_right_anim[1] = idle_top_right_anim[1];
	this->idle_top_right_anim[2] = idle_top_right_anim[2];
	this->idle_top_anim[0] = idle_top_anim[0];
	this->idle_top_anim[1] = idle_top_anim[1];
	this->idle_top_anim[2] = idle_top_anim[2];
	this->idle_top_left_anim[0] = idle_top_left_anim[0];
	this->idle_top_left_anim[1] = idle_top_left_anim[1];
	this->idle_top_left_anim[2] = idle_top_left_anim[2];
	this->idle_left_anim[0] = idle_left_anim[0];
	this->idle_left_anim[1] = idle_left_anim[1];
	this->idle_left_anim[2] = idle_left_anim[2];
	this->idle_down_left_anim[0] = idle_down_left_anim[0];
	this->idle_down_left_anim[1] = idle_down_left_anim[1];
	this->idle_down_left_anim[2] = idle_down_left_anim[2];
	this->idle_down_anim[0] = idle_down_anim[0];
	this->idle_down_anim[1] = idle_down_anim[1];
	this->idle_down_anim[2] = idle_down_anim[2];
	this->idle_down_right_anim[0] = idle_down_right_anim[0];
	this->idle_down_right_anim[1] = idle_down_right_anim[1];
	this->idle_down_right_anim[2] = idle_down_right_anim[2];
}

void Boss1Component::updateAnimation(const float& deltaTime) {

	if (player == nullptr) {
		std::cerr << "You forgot to assign player to Boss1!" << std::endl;
	}
	else {
		direction = glm::normalize(player->getPosition() - gameObject->getPosition());
	}
	float angle = glm::atan(direction.y, direction.x);
	float angle_deg = glm::degrees(angle);

	if (angle_deg < -157.5f) {
		bossAnimator->setAnimation(idle_left_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < -112.5f) {
		bossAnimator->setAnimation(idle_down_left_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < -67.5f) {
		bossAnimator->setAnimation(idle_down_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < -22.5f) {
		bossAnimator->setAnimation(idle_down_right_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < 22.5f) {
		bossAnimator->setAnimation(idle_right_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < 67.5f) {
		bossAnimator->setAnimation(idle_top_right_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < 112.5f) {
		bossAnimator->setAnimation(idle_top_anim[(int)(_phase) % 3], false);
	}
	else if (angle_deg < 157.5f) {
		bossAnimator->setAnimation(idle_top_left_anim[(int)(_phase) % 3], false);
	}
	else {
		bossAnimator->setAnimation(idle_left_anim[(int)(_phase) % 3], false);
	}
}

void Boss1Component::setPlayer(std::shared_ptr<GameObject> player) {
	this->player = player;
}