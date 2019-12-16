/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - null service for use in audio service provider
 */

#pragma once
#include "IAudioService.hpp"
#include <string>

class NullAudio : public IAudioService {

public:
	virtual void playOneshot(std::string soundName) { /* Do Nothing */ }
	virtual void playLooped(std::string filename) { /* Do Nothing */ }
	virtual void stopAll() { /* Do Nothing */ }
};