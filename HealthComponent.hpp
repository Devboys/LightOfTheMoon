/*
 * Created by Francesco Frassineti on 06/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Francesco Frassineti [06/12/19] - Basic creation
 */

#pragma once
#include "Component.hpp"

/*Class with information regarding the health of the GameObject*/
class HealthComponent : public Component {
public:
	explicit HealthComponent(GameObject* gameObject);
	virtual ~HealthComponent();

	void setMaxHealth(int amount);
	int getMaxHealth();
	void setCurrentHealth(int amount);
	int getCurrentHealth();
	void addHealth(int amount);
	void removeHealth(int amount);

	bool isAlive();

	/*Print current_healt / max_health*/
	void print();

private:
	int max_health;
	int current_health;
};