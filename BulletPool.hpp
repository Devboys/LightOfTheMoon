/*
 * Created by Alberto Giudice on 12/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [15/12/19] - Class interface decided
 * + Alberto Giudice [15/12/19] - Basic creation
 */

#pragma once

#include "GameObject.hpp"

class BulletPool {
public:
	/*Create pool for bullets with a linear movement*/
	BulletPool* createLinearPool();
	/*Create pool for bullets with the wave movement*/
	BulletPool* createWavePool();
	/*Create pool for bullets with the spiral movement*/
	BulletPool* createSpiralPool();

	void spawnPlayerLinearBullet(const glm::vec2 & position, const std::vector<sre::Sprite>& sprites, 
								const int& damage, const float& directionAngleDeg, const float& velocity);

	void spawnBossLinearBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites,
								const int& damage, const float& directionAngleDeg, const float& velocity);

	void spawnBossWaveBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
							const float& directionAngleDeg, const float& velocity, const float& amplitude = 1.0f, const float& frequency = 1.0f);

	void spawnBossSpiralBullet(const glm::vec2& position, const std::vector<sre::Sprite>& sprites, const int& damage,
							const float& minVelocity, const float& maxVelocity, const float& radiusExpansionRate, const bool& clockwise = true);

private:
	static const int POOL_SIZE = 100;
	std::shared_ptr<GameObject> linearBullets[POOL_SIZE];
	std::shared_ptr<GameObject> waveBullets[POOL_SIZE];
	std::shared_ptr<GameObject> spiralBullets[POOL_SIZE];
};