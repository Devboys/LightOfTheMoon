
#include "TileMapRenderer.hpp"
#include "LightOfTheMoon.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "AssetLocator.hpp"

//easy debug ;)
#include "iostream"

TileMapRenderer::TileMapRenderer() {

}


/* Caches sprites for use in tilemap
 * 
 * @param atlas : SpriteAtlas to source sprites from.
 */
void TileMapRenderer::loadSprites(std::shared_ptr<sre::SpriteAtlas> atlas) {
	
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

/* Loads the tilemap from a .json file. Tilemaps are read from JSON objects called "tileMap" which must be 2D arrays of integers
 *
 * @param filename : The name of a file (must be .json) in the program root directory.
 */
void TileMapRenderer::loadMap(std::string filename) {

	using namespace rapidjson;
	Document* docPointer = AssetLocator::getService()->getJsonAsset(filename);

	auto jsonArray = (*docPointer)["tileMap"].GetArray();

	//load tilemap as a 2-dimensional vector.
	for (auto& x : jsonArray) {
		std::vector<int> row;
		for (auto& y : x.GetArray()) {
			//GetArray() returns json generics, so we must use GetInt() to get actual value.
			row.push_back(y.GetInt());
		}
		tileMap.push_back(row);
	}
}

/* Adds the tilemap to the given spriteBatchBuilder as sprites.
 *
 * @param batchBuilder : where to add the sprites
 */
void TileMapRenderer::renderMap(sre::SpriteBatch::SpriteBatchBuilder& batchBuilder) {

	sre::Sprite sprite;

	//render 2D array as isometric tilemap
	for (int mapX = 0; mapX < tileMap.size(); mapX++) {
		for (int mapY = 0; mapY < tileMap[mapX].size(); mapY++) {

			//Tilemap describes type of tile.
			sprite = sprites.at(tileMap[mapX][mapY]);

			//place tile
			float spriteIsoX = (mapX - mapY) * (tileWidth / 2) * sprite.getScale().x;
			float spriteIsoY = (mapX + mapY) * (tileHeight / 2) * sprite.getScale().y;

			sprite.setPosition(glm::vec2(spriteIsoX, spriteIsoY));
			batchBuilder.addSprite(sprite);
		}
	}

}