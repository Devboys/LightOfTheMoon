
#include "DefaultAssetManager.hpp"
#include <iostream>

void DefaultAssetManager::loadAssets() {
	std::cout << "DEFAULT: assets loaded \n";
}

rapidjson::Document* DefaultAssetManager::getJsonAsset(std::string filename) {
	std::cout << "DEFAULT: json asset Gotten \n";
	return NULL;
}

Mix_Chunk* DefaultAssetManager::getSoundAsset(std::string filename) {
	return NULL;
	std::cout << "DEFAULT: sound asset Gotten \n";
}

void DefaultAssetManager::getSpriteAsset() {
	std::cout << "DEFAULT: sprite asset Gotten \n";
}