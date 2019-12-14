/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [08/12/19] - Basic implementation - Service locator based.
 * + Jeppe Faber [11/12/19] - Added SpriteAtlas loading.
 */

#pragma once
#include "IAssetService.hpp"
#include <map>

class GameAssetManager : public IAssetService{
public:
	~GameAssetManager();
	virtual void loadAssets();
	virtual rapidjson::Document* getJsonAsset(std::string filename);
	virtual Mix_Chunk* getSoundAsset(std::string filename);
	virtual std::shared_ptr<sre::SpriteAtlas> getSpriteAtlas(std::string jsonFilename);

private:

	void loadJson(std::string path);
	void loadSound(std::string path);
	void loadSpriteAtlas(std::string jsonPath, std::string pngPath);
	std::string convertPath(std::string path);

	std::map<std::string, rapidjson::Document*> jsonRegistry;
	std::map<std::string, Mix_Chunk*> soundRegistry;
	std::map<std::string, std::shared_ptr<sre::SpriteAtlas>> atlasRegistry;
};