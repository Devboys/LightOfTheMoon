/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [08/12/19] - Basic implementation - Singleton based
 */

#pragma once
#include "IAssetService.hpp"
#include <map>
#include <memory>
#include <string>
#include "sre/SpriteAtlas.hpp"

class GameAssetManager : public IAssetService{
public:
	~GameAssetManager();
	virtual void loadAssets();
	virtual rapidjson::Document* getJsonAsset(std::string filename);
	virtual Mix_Chunk* getSoundAsset(std::string filename);

private:

	void loadJson(std::string path);
	void loadSound(std::string path);
	std::string convertPath(std::string path);

	std::map<std::string, rapidjson::Document*> jsonRegistry;
	std::map<std::string, Mix_Chunk*> soundRegistry;
	std::map<std::string, sre::SpriteAtlas*> spriteRegistry;
};