/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
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

Boss1Component::Boss1Component(GameObject* gameObject) : Component(gameObject), _phase(Boss1Phase::First), _state(Boss1State::Idle) {
	bossHealth = gameObject->getComponent<HealthComponent>();
	bossAnimator = gameObject->getComponent<AnimatorComponent>();

	direction = glm::vec2(0, -1); // Start by facing south
	
	timeElapsedCurrentPhase = 0.0f;
	shootingTimer = 0.0f;
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

Boss1Component::~Boss1Component() {
	delete bulletPool;
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
		LightOfTheMoon::getInstance()->requestChangeState(GameState::GameOver);
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
		_phase = Boss1Phase::Second;
		_state = Boss1State::Idle;
		shootingCooldownFirstPattern = 0.3f;
		shootingCooldownSecondPattern = 0.3f;
		shootingCooldownThirdPattern = 0.15f;
		timeElapsedCurrentPhase = 0.0f;
		shootingTimer = 0.0f;
		bossHealth->print();
		std::cout << "Boss1: Transition to Phase 2" << std::endl;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();
	glm::vec2 direction = player->getPosition() - curPosition;
	float directionAngle = std::atan2f(direction.y, direction.x) * 180.0f / M_PI;

	bool bulletFired = false;
	/*LOGIC FOR THE CURRENT PHASE*/
	switch (_state) {
		case Boss1State::Idle:
			if (timeElapsedCurrentPhase > .5f)
				_state = Boss1State::FirstShotPattern;
			break;
		case Boss1State::FirstShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 30, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 30, 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownFirstPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 2.0f) > 1.0f)
				_state = Boss1State::SecondShotPattern;
			break;
		case Boss1State::SecondShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 15, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 15, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle + 45, 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, directionAngle - 45, 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownSecondPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 2.0f) < 1.0f)
				_state = Boss1State::FirstShotPattern;
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
		_phase = Boss1Phase::Third;
		_state = Boss1State::Idle;
		shootingCooldownFirstPattern = 0.3f;
		shootingCooldownSecondPattern = 0.3f;
		shootingCooldownThirdPattern = 0.8f;
		timeElapsedCurrentPhase = 0.0f;
		shootingTimer = 0.0f;
		bossHealth->print();
		std::cout << "Boss1: Transition to Phase 3" << std::endl;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();

	bool bulletFired = false;

	switch (_state) {
		case Boss1State::Idle:
			if (timeElapsedCurrentPhase > .5f)
				_state = Boss1State::FirstShotPattern;
			break;
		case Boss1State::FirstShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 22.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 67.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 112.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 157.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 202.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 247.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 292.5f, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 337.5f, .4f, .08f, 8.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownFirstPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 4.0f) > 1.0f)
				_state = Boss1State::SecondShotPattern;
			break;
		case Boss1State::SecondShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 0, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 45, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 90, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 135, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 180, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 225, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 270, .4f, .08f, 8.0f);
				bulletPool->spawnBossWaveBullet(curPosition, waveBulletSprites, 15, 315, .4f, .08f, 8.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownFirstPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 4.0f) > 2.0f)
				_state = Boss1State::ThirdShotPattern;
			break;
		case Boss1State::ThirdShotPattern:
			if (shootingTimer < 0.0f) { 
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 0 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 45 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 90 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 135 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 180 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 225 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 270 + (rand() % 21 - 10), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 315 + (rand() % 21 - 10), 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownThirdPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 4.0f) < 1.0f)
				_state = Boss1State::FirstShotPattern;
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
		bossHealth->print();
		_phase = Boss1Phase::Dead;
		_state = Boss1State::Idle;
	}

	// calculate useful positions and directions
	glm::vec2 curPosition = gameObject->getPosition();
	float offsetX = (float)(rand() % 101) / 100 - 0.5f;
	float offsetY = (float)(rand() % 101) / 100 - 0.5f;

	bool bulletFired = false;

	switch (_state) {
		case Boss1State::Idle:
			if (timeElapsedCurrentPhase > .5f)
				_state = Boss1State::FirstShotPattern;
			break;
		case Boss1State::FirstShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossSpiralBullet(curPosition + glm::vec2(offsetX, offsetY), spiralBulletSprites, 30, 40.0f, 150.0f, .05f, true);
				bulletFired = true;
				shootingTimer = shootingCooldownFirstPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 5.0f) > 1.0f)
				_state = Boss1State::SecondShotPattern;
			break;
		case Boss1State::SecondShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossSpiralBullet(curPosition + glm::vec2(offsetX, offsetY), spiralBulletSprites, 30, 40.0f, 150.0f, .05f, false);
				bulletFired = true;
				shootingTimer = shootingCooldownSecondPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 5.0f) > 2.0f)
				_state = Boss1State::ThirdShotPattern;
			break;
		case Boss1State::ThirdShotPattern:
			if (shootingTimer < 0.0f) {
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 0 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 15 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 30 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 45 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 60 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 75 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 90 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 105 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 120 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 135 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 150 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 165 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 180 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 195 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 210 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 225 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 240 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 255 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 270 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 285 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 300 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 315 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 330 + (rand() % 7 - 3), 50.0f);
				bulletPool->spawnBossLinearBullet(curPosition, linearBulletSprites, 10, 345 + (rand() % 7 - 3), 50.0f);
				bulletFired = true;
				shootingTimer = shootingCooldownThirdPattern;
			}
			if (fmodf(timeElapsedCurrentPhase, 5.0f) < 1.0f)
				_state = Boss1State::FirstShotPattern;
			break;
		default:
			break;
	}

	if (bulletFired)
		AudioLocator::getService()->playOneshot("Assets/Sounds/BossShootSound.wav");

	shootingTimer -= deltaTime;
	timeElapsedCurrentPhase += deltaTime;
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

void Boss1Component::setPlayer(std::shared_ptr<GameObject> player) {
	this->player = player;
}