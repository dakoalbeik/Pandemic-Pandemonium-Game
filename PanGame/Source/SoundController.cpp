#include "SoundController.h"


SoundController::~SoundController() {
	// free all sounds chunks
	for (auto& [sound, chunk] : soundsLibrary) {
		Mix_FreeChunk(chunk);
	}

	// free background music
	for (auto& [sound, music] : musicLibrary) {
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

bool SoundController::addSoundAsset(std::string name, std::string filePath) {

	Sound newSoundName = stringToSoundEnum(name);
	Mix_Chunk* newSound{ Mix_LoadWAV(filePath.c_str()) };
	newSound->volume = 15;
	soundsLibrary.emplace(newSoundName, newSound);
	if (soundsLibrary[newSoundName]) {
		return true;
	}
	else {
		auto soundItr = soundsLibrary.find(newSoundName);
		soundsLibrary.erase(soundItr);
		return false;
	}
}

bool SoundController::addMusicAsset(std::string name, std::string filePath) {

	Sound newMusicName = stringToSoundEnum(name);
	musicLibrary.emplace(newMusicName, Mix_LoadMUS(filePath.c_str()));
	if (musicLibrary[newMusicName]) {
		return true;
	}
	else {
		auto soundItr = musicLibrary.find(newMusicName);
		musicLibrary.erase(soundItr);
		return false;
	}
}

void SoundController::playMusic(Sound sound, int loop) {

	Mix_PlayMusic(musicLibrary[sound], loop);

}

void SoundController::playSound(Sound sound, int loop, int channelID) {
	Mix_PlayChannel(channelID, soundsLibrary[sound], loop);
}

void SoundController::pauseMusic() {
	Mix_PauseMusic();
}

Sound SoundController::stringToSoundEnum(std::string name) {
	if (name == "Bounce") {
		return Sound::BOUNCE;
	}
	else if (name == "Karen") {
		return Sound::KAREN;
	}
	else if (name == "BackgroundMusic") {
		return Sound::BACKGROUND_MUSIC;
	}
	else {
		printf("Failed to translate string to Enum");
		exit(1);
	}
}
