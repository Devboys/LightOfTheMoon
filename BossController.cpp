/*
 * Created by Alberto Giudice on 14/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [14/12/19] - Basic creation
 */

#include "BossController.hpp"

BossController::BossController(GameObject* gameObject) : Component(gameObject), _phase(BossPhase::Idle) {}