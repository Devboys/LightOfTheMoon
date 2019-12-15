
#include "TileMap.hpp"
#include "LightOfTheMoon.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include "AssetLocator.hpp"

//easy debug ;)
#include "iostream"
#include "PhysicsComponent.hpp"
#define TILE_SPRITE_SCALE 0.002f
#define COLLIDER_SCALE TILE_SPRITE_SCALE * 100

TileMap::TileMap() {

}


/* Caches sprites for use in tilemap
 * 
 * @param atlas : SpriteAtlas to source sprites from.
 */
void TileMap::loadSprites(std::shared_ptr<sre::SpriteAtlas> atlas) {
	sprites.clear(); //Delete the old ones

	//cache all sprites for later use
	sprites.push_back(atlas->get("top-tile-blue.png")); //0
	sprites.push_back(atlas->get("top-tile-cyan.png")); //1
	sprites.push_back(atlas->get("top-tile-gold.png")); //2

	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].setScale({ TILE_SPRITE_SCALE, TILE_SPRITE_SCALE });
	}

	tileWidth = sprites[0].getSpriteSize().x;
	tileHeight = sprites[0].getSpriteSize().y / 2; //iso sprites are half as tall as they are wide.
}

/* Loads the tilemap from a .json file. Tilemaps are read from JSON objects called "tileMap" which must be 2D arrays of integers
 *
 * @param filename : The name of a file (must be .json) in the program root directory.
 */
void TileMap::loadMap(std::string filename) {

	tileMap.clear();

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

void TileMap::clearMap() {
	this->tileMap.clear();
	this->tileHeight = 0;
	this->tileWidth = 0;
}

/* Prints the tilemap indexes in a matrix form
 *
 */
void TileMap::printMap() {
	for (std::vector<int>& x : tileMap) {
		std::cout << "|";
		for (int& y : x) {
			std::cout << " " << y << "\t|";
		}
		std::cout << std::endl;
	}
}

/*Generate every necessary edge colliders for each tile*/
void TileMap::generateColliders() {
	std::vector<std::vector<bool>> borderCells = calculateBorderCells(); //Temporary variable where the borders of the tilemap will be saved

	for (int i = 0; i < borderCells.size(); i++) {
		for (int j = 0; j < borderCells[0].size(); j++) {
			if (borderCells[i][j]) {
				if(i != 0 && !borderCells[i - 1][j])
					generateEdgeBottomLeft(i, j);
				if (i != borderCells.size() -1  && !borderCells[i + 1][j])
					generateEdgeTopRight(i, j);
				if(j != 0 && !borderCells[i][j -1])
					generateEdgeBottomRight(i, j);
				if (j != borderCells[0].size() - 1 && !borderCells[i][j + 1])
					generateEdgeTopLeft(i, j);
			}
		}
	}
}

void TileMap::generateEdgeBottomLeft(int i, int j) {
	//Create collider object
	std::shared_ptr<GameObject> go = LightOfTheMoon::getInstance()->createGameObject();
	go->setPosition(glm::vec2(0, 0));
	go->name = "Wall";
	auto phys = go->addComponent<PhysicsComponent>();

	float jc = j - (tileMap[0].size()+2) / 2.0 - 0.5f;
	float ic = i - (tileMap.size()+2) / 2.0 - 0.5f;

	float edgeX_i = (-jc + ic - 0) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_i = (jc + ic - 2 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	float edgeX_f = (-jc + ic - 1) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_f = (jc + ic - 1 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	phys->initEdge(b2_staticBody, { 0, 0 }, { edgeX_i, edgeY_i }, { edgeX_f, edgeY_f });
}

void TileMap::generateEdgeBottomRight(int i, int j) {
	//Create collider object
	std::shared_ptr<GameObject> go = LightOfTheMoon::getInstance()->createGameObject();
	go->setPosition(glm::vec2(0, 0));
	go->name = "Wall";
	auto phys = go->addComponent<PhysicsComponent>();

	float jc = j - (tileMap[0].size() + 2) / 2.0 - 0.5f;
	float ic = i - (tileMap.size() + 2) / 2.0 - 0.5f;

	float edgeX_i = (-jc + ic + 0) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_i = (jc + ic + -2 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	float edgeX_f = (-jc + ic + 1) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_f = (jc + ic - 1 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	phys->initEdge(b2_staticBody, { 0, 0 }, { edgeX_i, edgeY_i }, { edgeX_f, edgeY_f });
}

void TileMap::generateEdgeTopLeft(int i, int j) {
	//Create collider object
	std::shared_ptr<GameObject> go = LightOfTheMoon::getInstance()->createGameObject();
	go->setPosition(glm::vec2(0, 0));
	go->name = "Wall";
	auto phys = go->addComponent<PhysicsComponent>();

	float jc = j - (tileMap[0].size() + 2) / 2.0 - 0.5f;
	float ic = i - (tileMap.size() + 2) / 2.0 - 0.5f;

	float edgeX_i = (-jc + ic - 1) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_i = (jc + ic + -1 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	float edgeX_f = (-jc + ic + 0) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_f = (jc + ic - 0 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	phys->initEdge(b2_staticBody, { 0, 0 }, { edgeX_i, edgeY_i }, { edgeX_f, edgeY_f });
}

void TileMap::generateEdgeTopRight(int i, int j) {
	//Create collider object
	std::shared_ptr<GameObject> go = LightOfTheMoon::getInstance()->createGameObject();
	go->setPosition(glm::vec2(0, 0));
	go->name = "Wall";
	auto phys = go->addComponent<PhysicsComponent>();

	float jc = j - (tileMap[0].size() + 2) / 2.0 - 0.5f;
	float ic = i - (tileMap.size() + 2) / 2.0 - 0.5f;

	float edgeX_i = (-jc + ic + 1) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_i = (jc + ic - 1 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	float edgeX_f = (-jc + ic - 0) * (tileWidth / 2) * COLLIDER_SCALE;
	float edgeY_f = (jc + ic + 0 + 2) * (tileHeight / 2) * COLLIDER_SCALE;
	phys->initEdge(b2_staticBody, { 0, 0 }, { edgeX_i, edgeY_i }, { edgeX_f, edgeY_f });
}


/*Returns a matrix with information regarding the empty tiles of the tilemap, including the extended borders.
* tileMap has size n*m, the function return a (n+1)*(m+1) matrix
*/
std::vector<std::vector<bool>> TileMap::calculateBorderCells() {
	std::vector<std::vector<bool>> borderCells;
	//Init border cells
	for (int i = 0; i < tileMap.size() + 2; i++) {
		std::vector<bool> row;
		for (int j = 0; j < tileMap[0].size() + 2; j++) {
			row.push_back(true);
		}
		borderCells.push_back(row);
	}

	for (int i = 0; i < tileMap.size(); i++) {
		for (int j = 0; j < tileMap[0].size(); j++) {
			#define i_f i + 1
			#define j_f j + 1
			if (tileMap[i][j] >= 0) {
				//Solid tile
				//Check down
				if (i != 0)
					borderCells[i_f - 1][j_f] = tileMap[i - 1][j] < 0;
				//Check up
				if (i != tileMap.size() - 1)
					borderCells[i_f + 1][j_f] = tileMap[i + 1][j] < 0;
				//Check left
				if (j != 0)
					borderCells[i_f][j_f - 1] = tileMap[i][j - 1] < 0;
				//Check right
				if (j != tileMap[0].size() - 1)
					borderCells[i_f][j_f + 1] = tileMap[i][j + 1] < 0;
			}
			else {
				//Empty tile -> continue
			}
		}
	}


	return borderCells;
}

/* Adds the tilemap to the given spriteBatchBuilder as sprites.
 *
 * @param batchBuilder : where to add the sprites
 */
void TileMap::renderMap(sre::SpriteBatch::SpriteBatchBuilder& batchBuilder) {

	sre::Sprite sprite;
	//render 2D array as isometric tilemap
	for (int mapX = 0; mapX < tileMap.size(); mapX++) {
		for (int mapY = 0; mapY < tileMap[mapX].size(); mapY++) {
			if (tileMap[mapX][mapY] >= 0) {//Check if it's a rendered tile (-1 is the empty tile)
				//Tilemap describes type of tile.
				sprite = sprites.at(tileMap[mapX][mapY]);

				float centeredX;
				if (tileMap[0].size() % 2 == 0) {
					centeredX = mapX - tileMap[0].size() / 2.0 - 0.5f;
				}
				else {
					centeredX = mapX - (tileMap[0].size()-1) / 2.0 - 1;
				}
				float centeredY;
				if (tileMap.size() % 2 == 0) {
					centeredY = mapY - tileMap.size() / 2.0 - 0.5f;
				}
				else {
					centeredY = mapY - (tileMap.size() - 1) / 2.0 - 1;
				}

				//place tile
				float spriteIsoX = (centeredX - centeredY) * (tileWidth / 2) * sprite.getScale().x;
				float spriteIsoY = (centeredX + centeredY) * (tileHeight / 2) * sprite.getScale().y;

				sprite.setPosition(glm::vec2(spriteIsoX, spriteIsoY));
				batchBuilder.addSprite(sprite);
			}
		}
	}
}