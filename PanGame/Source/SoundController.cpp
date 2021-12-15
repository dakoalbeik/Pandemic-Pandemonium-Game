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
	soundsLibrary.emplace(newSoundName, Mix_LoadWAV(filePath.c_str()));
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

	// only if music is playing
	if (Mix_PlayingMusic()) {
		//fade out music over half a second and then halt the music
		Mix_FadeOutMusic(500);
	}
}

void SoundController::resumeMusic(Sound sound) {

	// fade in music over half a second and then loop like playMusic
	Mix_FadeInMusic(musicLibrary[sound], -1, 500);
}

void SoundController::haltMusic()
{
	// hard stops music (different than pausing)
	Mix_HaltMusic();
}

void SoundController::haltSound()
{
	// hard stops music (different than pausing)
	Mix_HaltChannel(-1);
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
	else if (name == "GameOver") {
		return Sound::GAME_OVER;
	}
	else if (name == "BossMusic") {
		return Sound::BOSS_MUSIC;
	}
	else if (name == "ItemCollect") {
		return Sound::ITEM_COLLECT;
	}
	else if (name == "Virus") {
		return Sound::VIRUS;
	}
	else {
		printf("Failed to translate string to Enum");
		exit(1);
	}
}
