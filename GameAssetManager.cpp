
#include "GameAssetManager.hpp"
#include <iostream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "rapidjson/error/en.h"
#include "sre/Texture.hpp"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING // this is necessary to use the below experimental feature
#include <experimental/filesystem> //experimental feature because actual feature is in C++17 and later only
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

	// Sprite Atlases are loaded from 2 files: json and png. 
	// These are loaded as a single asset, so we store them for further parsing instead of loading them immediatly.
	std::vector<fs::path> jsonPaths;
	std::vector<fs::path> pngPaths;

	for (const auto& entry : fs::recursive_directory_iterator(dirPath)) {
		
		fs::path path = entry.path();
		fs::path extension = path.extension();

		if (!extension.empty()) {
			if (extension.string() == ".wav") {
				loadSound(path.string());
			}
			else if (extension.string() == ".json") {
				jsonPaths.push_back(path);
			}
			else if (extension.filename().string() == ".png") {
				pngPaths.push_back(path);
			}
		}
	}

	//look for spriteAtlases in the loaded files.
	for (std::vector<fs::path>::iterator jsonItr = jsonPaths.begin(); jsonItr != jsonPaths.end(); ++jsonItr){

		fs::path json = *jsonItr;
		json.replace_extension("");
		
		fs::path png;
		bool pngFound = false;

		for (std::vector<fs::path>::iterator pngItr = pngPaths.begin(); pngItr != pngPaths.end(); ++pngItr) {
			png = *pngItr;
			png.replace_extension("");

			if ((json.compare(png) == 0)) {
				if (!pngFound) {
					pngFound = true;
					loadSpriteAtlas(jsonItr->string(), pngItr->string());
				}
				else {
					std::cout << "DUPLICATE PNG FOUND FOR; " << jsonItr->string();
				}
			}
		}
		if (!pngFound) {
			loadJson(jsonItr->string());
		}
	}

	std::cout << "\nGAME: assets LOADED \n";
}

rapidjson::Document* GameAssetManager::getJsonAsset(std::string filename) {

	filename = convertPath(filename);
	return jsonRegistry.at(filename);
}

Mix_Chunk* GameAssetManager::getSoundAsset(std::string filename) {

	filename = convertPath(filename);
	return soundRegistry.at(filename);
}

std::shared_ptr<sre::SpriteAtlas> GameAssetManager::getSpriteAtlas(std::string filename) {

	filename = convertPath(filename);
	return atlasRegistry.at(filename);

}


#pragma region Load utilities

void GameAssetManager::loadJson(std::string path) {
	using namespace std;
	using namespace rapidjson;

	std::cout << "    JSON: " << path << std::endl;

	ifstream fis(path);
	IStreamWrapper isw(fis);
	Document* doc = new Document(); //document stored on heap to prevent cleanup.
	ParseResult result = doc->ParseStream(isw);
	if (!result) {
		printf( "JSON parse error: %s (%u)", GetParseError_En(result.Code()), result.Offset());
	}
	
	jsonRegistry.insert(pair<string, Document*>(path, doc));
}

void GameAssetManager::loadSound(std::string path) {
	using namespace std;

	std::cout << "    SOUND: " << path << std::endl;

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

void GameAssetManager::loadSpriteAtlas(std::string jsonPath, std::string pngPath) {
	using namespace std;
	using namespace sre;
	std::cout << "    ATLAS: " << jsonPath << std::endl;
	std::cout << "      - PNG: " << pngPath << std::endl;

	shared_ptr<SpriteAtlas> atlasPtr = SpriteAtlas::create(jsonPath, Texture::create()
		.withFile(pngPath)
		.withFilterSampling(false)
		.build());

	atlasRegistry.insert(pair<string, shared_ptr<SpriteAtlas>>(jsonPath, atlasPtr));
}

std::string GameAssetManager::convertPath(std::string path) {

	//asset registries expect '\' as hierarchy delimiters, so convert variations of this (namely '/')
	std::string convertedPath = path;

	size_t index = 0;
	while (true) {	//replace algorithm from: https://stackoverflow.com/a/4643526
		/* Locate the substring to replace. */
		index = convertedPath.find("/", index);
		if (index == std::string::npos) break;

		/* Make the replacement. */
		convertedPath.replace(index, 1, "\\"); //replace with '\'

		/* Advance index forward so the next iteration doesn't pick it up as well. */
		index += 1;
	}

	return convertedPath;
}

#pragma endregion