/*
 * Created by Alberto Giudice on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Alberto Giudice [05/12/19] - Basic creation
 */

#pragma once

#include "Component.hpp"
#include "sol.hpp"
#include <SDL_events.h>

class ScriptComponent : public Component {
public:
	explicit ScriptComponent(GameObject* gameObject);

	bool init(std::string luaScript, bool isFilename = false);

	// The function should return true, if the key event is consumed. This prevents other components to receive the event.
	bool onKey(SDL_Event& event) override;

	void update(float deltaTime) override;

private:
	std::string luaString;
	bool isLoaded = false;

	std::string error;
	sol::state lua;
	sol::protected_function lua_update;
	sol::protected_function lua_onKeyDown;
	sol::protected_function lua_onKeyUp;
};