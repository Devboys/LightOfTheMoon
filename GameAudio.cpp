

#include "GameAudio.hpp"
#include "AssetLocator.hpp"

GameAudio::GameAudio() {

	//init SDL tempSound library
	Mix_OpenAudio(
		22050,				// Frequency
		MIX_DEFAULT_FORMAT, // Mix format
		2,					// Number of channels
		4096 );				// chunkSize (bytes allocated pr. sample aka Mix_Chunk)
}

GameAudio::~GameAudio() {

	//clean up sdl_mixer
	Mix_CloseAudio();
}

/*
 * Play single sound by filename.
 */
void GameAudio::playOneshot(std::string filename) {
	
	Mix_Chunk* sound = AssetLocator::getService()->getSoundAsset(filename);

	//play cached sound at next free mixer channel
	Mix_PlayChannel(-1, sound, 0);

}

/* 
 * Play a looped sound by filename
 */
void GameAudio::playLooped(std::string filename) {

	Mix_Chunk* sound = AssetLocator::getService()->getSoundAsset(filename);
	Mix_PlayChannel(-1, sound, -1);
}

/*
 * Stop all playback
 */
void GameAudio::stopAll() {
	//halts playback on all mixer channels
	Mix_HaltChannel(-1);
}