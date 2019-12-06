/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#pragma once
#include "Component.hpp"
#include "DamageComponent.hpp"

 //Make a FIXED amount of damage
class FixedDamageComponent : public DamageComponent {
public:
	explicit FixedDamageComponent(GameObject* gameObject);
	virtual ~FixedDamageComponent();

	int getDamage();
	void setDamage(int amount);

private:
	int damage;
};