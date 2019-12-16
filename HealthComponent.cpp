/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#include "HealthComponent.hpp"
#include "AudioLocator.hpp"
#include <iostream>

HealthComponent::HealthComponent(GameObject* gameObject)
	: Component(gameObject)
{

}

HealthComponent::~HealthComponent() {

}

void HealthComponent::setMaxHealth(int amount) {
	if (amount <= 0)
		throw "Setting a non positive amount of health as Maximum health.";

	max_health = amount;
}

int HealthComponent::getMaxHealth() {
	return max_health;
}

void HealthComponent::setCurrentHealth(int amount) {
	if (amount < 0)
		throw "Setting a negative amount of health as Current health.";

	current_health = amount;
}

int HealthComponent::getCurrentHealth() {
	return current_health;
}

void HealthComponent::addHealth(int amount) {
	if (amount < 0)
		throw "Adding a negative amount of health!";

	current_health += amount;
	if (current_health > max_health)
		current_health = max_health;
}

void HealthComponent::removeHealth(int amount) {
	if (amount < 0)
		throw "Removing a negative amount of health!";

	if(amount > 0)
		AudioLocator::getService()->playOneshot("Assets/Sounds/HurtSound.wav");

	current_health -= amount;
	if (current_health < 0)
		current_health = 0;
}


bool HealthComponent::isAlive() {
	return current_health > 0;
}

void HealthComponent::print() {
	std::cout << current_health << "/" << max_health << std::endl;
}