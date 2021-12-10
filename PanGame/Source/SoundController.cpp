#include "SoundController.h"


SoundController::~SoundController() {
	// free all sounds chunks
	for (auto& [sound, chunk] : soundsLibrary) {
		Mix_FreeChunk(chunk);
	}

	// free background music
	if (music) {
		Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
}

bool SoundController::initialize(int frequencey, Uint16 format, int channels, int chunksize, int numChannels) {
	if (Mix_OpenAudio(frequencey, format, channels, chunksize) < 0) {
		return false;
	}
	else {
		// allocate channels
		Mix_AllocateChannels(numChannels);
		return true;
	}
}

bool SoundController::addSoundAsset(Sound sound, std::string filePath) {
	//Mix_Chunk* tempChunk = Mix_LoadWAV(filePath.c_str());
	soundsLibrary.emplace(sound, Mix_LoadWAV(filePath.c_str()));
	if (soundsLibrary[sound]) {
		return true;
	}
	else {
		auto soundItr = soundsLibrary.find(sound);
		soundsLibrary.erase(soundItr);
		return false;
	}
}

bool SoundController::initializeMusic(std::string filePath)
{
	if (music = Mix_LoadMUS(filePath.c_str())) {
		return true;
	}
	else {
		return false;
	}

}

void SoundController::playBackgroundMusic(int loop) {
	if (music) {
		Mix_PlayMusic(music, loop);
	}
}

void SoundController::playSound(Sound sound, int loop, int channelID) {
	Mix_PlayChannel(channelID, soundsLibrary[sound], loop);
}
