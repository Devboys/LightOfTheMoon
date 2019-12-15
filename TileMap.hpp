/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [05/12/19] - Basic implementation - Tilemap reads json file for structure.
 */

#pragma once

#include <vector>
#include <string>
#include "sre/Sprite.hpp"
#include "sre/SpriteBatch.hpp"

class TileMap{
public:
	explicit TileMap();

	/*Load the tileMap from a json file*/
	void loadMap(std::string filename);
	void clearMap();

	void printMap();//Prints the tilemap indexes in a matrix form

	void renderMap(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

	void loadSprites(std::shared_ptr<sre::SpriteAtlas>);

	/*Generate every necessary edge colliders for each tile*/
	void generateColliders();


private:
	/*Returns a matrix with information regarding the empty tiles of the tilemap, including the extended borders.
	* tileMap has size n*m, the function return a (n+1)*(m+1) matrix
	*/
	std::vector<std::vector<bool>> calculateBorderCells();
	void generateEdgeBottomLeft(int i, int j);
	void generateEdgeBottomRight(int i, int j);
	void generateEdgeTopLeft(int i, int j);
	void generateEdgeTopRight(int i, int j);

	std::vector<std::vector<int>> tileMap;
	int tileHeight;
	int tileWidth;

	std::vector<sre::Sprite> sprites;

};