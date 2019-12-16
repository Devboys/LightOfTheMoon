/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [16/12/19] - Creation and implementation
 */

#include "BossComponent.hpp"

BossComponent::BossComponent(GameObject* gameObject) : Component(gameObject), _phase(BossPhase::First), _state(BossState::Idle) {}

BossComponent::~BossComponent() {
	delete bulletPool;
}

void BossComponent::update(float deltaTime) {}