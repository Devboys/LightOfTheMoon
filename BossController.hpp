/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"

enum class BossPhase {
	Idle,
	First,
	Second,
	Third,
	Dead
};

class BossController : public Component {
public:
	explicit BossController(GameObject* gameObject);

	virtual void initBoss() = 0;

private:
	BossPhase _phase;
};