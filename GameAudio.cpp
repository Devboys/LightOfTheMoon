

#include "GameAudio.hpp"
#include <iostream>

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
	
	Mix_Chunk* sound = sounds[filename];

	if (sound == NULL) {
		loadSound(filename);
		sound = sounds.at(filename);
	}
	//play cached sound at next free mixer channel
	Mix_PlayChannel(-1, sound, 0);

}

/*
 * Stop all playback
 */
void GameAudio::stopAll() {
	//halts playback on all mixer channels
	Mix_HaltChannel(-1);
}

//TODO: THIS SHOULD BE MOVED TO A RESOURCEMANAGER
//loads sound asset.
void GameAudio::loadSound(std::string filename) {

	//convert std::string to char* (c-string) because MIX_LoadWAV() expects this.
	char* convertedString = &filename[0];
	Mix_Chunk* sound = Mix_LoadWAV(convertedString);
	
	//print SDL_mixer error if wav-file was not found
	if (sound == NULL) {
		printf("Mix_LoadWAV error for '%s': %s \n", &filename[0], Mix_GetError());
	}
	else {

		//if sound isn't cached already, insert it.
		if (sounds.find(filename) == sounds.end()) { 
			sounds.insert(std::pair<std::string, Mix_Chunk*>(filename, sound));
		}
		//if sound is already cached replace it.
		else {
			sounds[filename] = sound;
		}

	}
}