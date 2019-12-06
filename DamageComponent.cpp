/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#include "DamageComponent.hpp"
#include "HealthComponent.hpp"
#include "GameObject.hpp"

DamageComponent::DamageComponent(GameObject* gameObject)
	: Component(gameObject)
{

}

DamageComponent::~DamageComponent() {

}

void DamageComponent::onCollisionStart(PhysicsComponent *comp) {
	std::shared_ptr<HealthComponent> hp = comp->getGameObject()->getComponent<HealthComponent>();

	if (&(*hp) != nullptr) {
		hp->removeHealth(this->getDamage());
	}
		

}

void DamageComponent::onCollisionEnd(PhysicsComponent *comp) {

}