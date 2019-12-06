
#include "Level.hpp"
#include "LightOfTheMoon.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>

//easy debug ;)
#include "iostream"

Level::Level() {

}

void Level::loadSprites(std::shared_ptr<sre::SpriteAtlas> atlas) {
	
	//cache all sprites for later use
	sprites.push_back(atlas->get("top-tile-blue.png")); //0
	sprites.push_back(atlas->get("top-tile-cyan.png")); //1
	sprites.push_back(atlas->get("top-tile-gold.png")); //2

	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].setScale({ 0.001f, 0.001f });
	}

	tileWidth = sprites[0].getSpriteSize().x;
	tileHeight = sprites[0].getSpriteSize().y / 2; //iso sprites are half as tall as they are wide.
}

void Level::loadLevel(std::string filename) {

	using namespace rapidjson;
	std::ifstream fis(filename);
	IStreamWrapper isw(fis);
	Document d;
	d.ParseStream(isw);

	//load tilemap as a 2-dimensional vector.
	for (auto& x : d["tileMap"].GetArray()) {
		std::vector<int> row;
		for (auto& y : x.GetArray()) {
			//GetArray() returns generics, so we must use GetInt() to get actual value.
			row.push_back(y.GetInt());
		}
		tileMap.push_back(row);
	}
}

void Level::renderLevel(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder) {

	sre::Sprite sprite;

	//render 2D array isometrically
	for (int mapX = 0; mapX < tileMap.size(); mapX++) {
		for (int mapY = 0; mapY < tileMap[mapX].size(); mapY++) {

			//Tilemap describes type of tile.
			sprite = sprites.at(tileMap[mapX][mapY]);

			//place tile
			float spriteIsoX = (mapX - mapY) * (tileWidth / 2) * sprite.getScale().x;
			float spriteIsoY = (mapX + mapY) * (tileHeight / 2) * sprite.getScale().y;

			sprite.setPosition(glm::vec2(spriteIsoX, spriteIsoY));
			spriteBatchBuilder.addSprite(sprite);
		}
	}

}