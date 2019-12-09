
/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - Simple service interface for use in Service locator
 */

#pragma once
#include <string>
#include "SDL_mixer.h"
#include "rapidjson/document.h"

class IAssetService {
public:
	virtual ~IAssetService() {}
	virtual void loadAssets() = 0;
	virtual rapidjson::Document* getJsonAsset(std::string filename) = 0;
	virtual Mix_Chunk* getSoundAsset(std::string filename) = 0;
};

