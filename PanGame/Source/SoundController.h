#pragma once
#include "SDL_mixer.h"
#include <map>
#include <string>


enum class Sound { BOUNCE, KAREN, BACKGROUND_MUSIC, GAME_OVER, BOSS_MUSIC };
class SoundController {
public:
	SoundController() {};
	~SoundController();
	bool initialize(int frequencey, Uint16 format, int channels, int chunksize, int numChannels = 100);
	bool addSoundAsset(std::string name, std::string filePath);
	bool addMusicAsset(std::string name, std::string filePath);
	void playMusic(Sound sound, int loop = -1);
	void playSound(Sound sound, int loop, int channelID = -1);
	void pauseMusic();
	void resumeMusic(Sound sound);
	void haltMusic();
	void haltSound();
private:
	Sound stringToSoundEnum(std::string name);
	std::map<Sound, Mix_Chunk*> soundsLibrary;
	std::map<Sound, Mix_Music*> musicLibrary;
};