/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#include "FixedDamageComponent.hpp"
#include "GameObject.hpp"

FixedDamageComponent::FixedDamageComponent(GameObject* gameObject)
	: DamageComponent(gameObject)
{

}

FixedDamageComponent::~FixedDamageComponent() {

}

int FixedDamageComponent::getDamage() {
	return damage;
}

void FixedDamageComponent::setDamage(int amount) {
	if (amount < 0)
		throw "Damage amount has to be non negative.";

	damage = amount;
}