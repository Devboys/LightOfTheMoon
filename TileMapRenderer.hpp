/*
 * Created by Jeppe Faber on 05/12/2019.
 * LIST OF EDITS (reverse chronological order - add last on top):
 * +
 * + Jeppe Faber [05/12/19] - Basic implementation - Tilemap reads json file for sctructure.
 */

#pragma once

#include <vector>
#include <string>
#include "sre/Sprite.hpp"
#include "sre/SpriteBatch.hpp"

class TileMapRenderer {
public:
	explicit TileMapRenderer();

	void loadLevel(std::string filename);

	void renderLevel(sre::SpriteBatch::SpriteBatchBuilder& spriteBatchBuilder);

	void loadSprites(std::shared_ptr<sre::SpriteAtlas>);

private:
	std::vector<std::vector<int>> tileMap;
	int tileHeight;
	int tileWidth;

	std::vector<sre::Sprite> sprites;

};