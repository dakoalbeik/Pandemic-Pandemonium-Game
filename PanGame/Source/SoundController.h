#pragma once
#include "SDL_mixer.h"
#include <map>
#include <string>


enum class Sound { BOUNCE, KAREN };
class SoundController {
public:
	SoundController() {};
	~SoundController();
	bool initialize(int frequencey, Uint16 format, int channels, int chunksize, int numChannels = 100);
	bool addSoundAsset(Sound sound, std::string filePath);
	bool initializeMusic(std::string filePath);
	void playBackgroundMusic(int loop = -1);
	void playSound(Sound sound, int loop, int channelID = -1);
private:
	std::map<Sound, Mix_Chunk*> soundsLibrary;
	Mix_Music* music{ nullptr };
};