
#include "DefaultAssetManager.hpp"
#include <iostream>

void DefaultAssetManager::loadAssets() {
	std::cout << "DEFAULT: assets loaded \n";
}

rapidjson::Document* DefaultAssetManager::getJsonAsset(std::string filename) {
	std::cout << "DEFAULT: json asset Gotten \n";
	return nullptr;
}

Mix_Chunk* DefaultAssetManager::getSoundAsset(std::string filename) {
	std::cout << "DEFAULT: sound asset Gotten \n";
	return nullptr;
}

std::shared_ptr<sre::SpriteAtlas> DefaultAssetManager::getSpriteAtlas(std::string jsonPath) {
	std::cout << "DEFAULT: sprite asset Gotten \n";
	return nullptr;
}