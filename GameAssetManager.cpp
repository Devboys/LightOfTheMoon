
#include "GameAssetManager.hpp"
#include <iostream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "rapidjson/error/en.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // this is necessary to use the below experimental feature
#include <experimental/filesystem> //experimental feature because actual feature is C++17
namespace fs = std::experimental::filesystem;

GameAssetManager::~GameAssetManager() 
{
	//cleanup json document map from heap
	for (std::map<std::string, rapidjson::Document*>::iterator itr = jsonRegistry.begin(); itr != jsonRegistry.end(); itr++)
	{
		delete itr->second;
	}
}

//load assets from assets folder
void GameAssetManager::loadAssets() {

	std::cout << "Loading Assets: " << std::endl;
	//get paths of every asset in Assets folder
	std::string dirPath = "Assets";

	for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
		
		fs::path path = entry.path();
		fs::path extension = path.extension();
		if (!extension.empty()) {
			std::cout << "    " << path.string() << std::endl;

			if (extension.string() == ".json") {
				loadJson(path.string());
			}
			else if (extension.string() == ".wav") {
				loadSound(path.string());
			}
			/*else if (extension.string() == ".png") {
				loadSpritesheet(path.string());
			}*/
		}
	}
	std::cout << "GAME: assets LOADED \n";
}

rapidjson::Document* GameAssetManager::getJsonAsset(std::string filename) {

	filename = convertPath(filename);
	return jsonRegistry.at(filename);
}

Mix_Chunk* GameAssetManager::getSoundAsset(std::string filename) {

	filename = convertPath(filename);
	return soundRegistry.at(filename);
}


#pragma region Load utilities

void GameAssetManager::loadJson(std::string path) {

	using namespace rapidjson;
	std::ifstream fis(path);
	IStreamWrapper isw(fis);
	Document* doc = new Document(); //document stored on heap to prevent cleanup.
	ParseResult result = doc->ParseStream(isw);
	if (!result) {
		printf( "JSON parse error: %s (%u)", GetParseError_En(result.Code()), result.Offset());
	}
	
	jsonRegistry.insert(std::pair<std::string, Document*>(path, doc));
}

void GameAssetManager::loadSound(std::string path) {

	//convert std::string to char* (c-string) because MIX_LoadWAV() expects this.
	char* convertedString = &path[0];
	Mix_Chunk* sound = Mix_LoadWAV(convertedString);

	//print SDL_mixer error if wav-file was not found
	if (sound == NULL) {
		printf("Mix_LoadWAV error for '%s': %s \n", &path[0], Mix_GetError());
	}
	else {

		soundRegistry.insert(std::pair<std::string, Mix_Chunk*>(path, sound));
	}

}

std::string GameAssetManager::convertPath(std::string path) {

	//asset registries expect '\\' as hierarchy delimiters, so convert variations of this (namely '/')
	std::string convertedPath = path;

	size_t index = 0;
	while (true) {	//replace algorithm from: https://stackoverflow.com/a/4643526
		/* Locate the substring to replace. */
		index = convertedPath.find("/", index);
		if (index == std::string::npos) break;

		/* Make the replacement. */
		convertedPath.replace(index, 1, "\\");

		/* Advance index forward so the next iteration doesn't pick it up as well. */
		index += 2;
	}

	return convertedPath;
}

#pragma endregion