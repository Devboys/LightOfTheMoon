/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [07/12/19] - Basic implementation - default asset manager - should return nothing or default values
 */

#pragma once
#include "IAssetService.hpp"

class DefaultAssetManager : public IAssetService {

public:
	virtual void loadAssets();
	virtual rapidjson::Document* getJsonAsset(std::string filename);
	virtual Mix_Chunk* getSoundAsset(std::string filename);
	virtual void getSpriteAsset();

private:
	rapidjson::Document* defaultJson;
	Mix_Chunk* defaultSound;

};