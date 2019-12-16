/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - Simple service interface for use in Service locator
 */

#pragma once
#include <string>

class IAudioService {
public:
	virtual ~IAudioService() {}
	virtual void playOneshot(std::string soundName) = 0;
	virtual void playLooped(std::string filename) = 0;
	virtual void stopAll() = 0;
};

