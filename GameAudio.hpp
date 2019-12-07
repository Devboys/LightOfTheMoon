/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - audio service provider. Plays sounds & all that jazz
 */

#pragma once
#include <string>
#include "AudioService.hpp"
#include "SDL_mixer.h"
#include <map>

class GameAudio : public AudioService {

public:
	GameAudio();
	~GameAudio();

	virtual void playOneshot(std::string soundName);
	virtual void stopAll();

private:
	void loadSound(std::string filename);

	std::map<std::string, Mix_Chunk*> sounds;
};
