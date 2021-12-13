#pragma once
#include "Vector2D.h"
#include "Initializers.h"
#include <vector>
#include <random>


struct PlatformPreset {
	Vector2D platformPosition{ 0,0 };
	bool hasItem{ false };
	Item itemName{ Item::NONE };
};

class RandomHandler {
public:
	static std::random_device seed;
	static std::default_random_engine e;
	RandomHandler();
	~RandomHandler();


	// returns a vector of random positions
	std::vector<PlatformPreset>& getRandomPositions();
private:

	// stores the random positions 
	std::vector<PlatformPreset> positions;

	// return random vector based on previous Y
	Vector2D getRandPosition(int currentY);

	// highest y position for new platforms
	const int maxY{ -600 };
	// max disntace between platforms
	const int maxDistanceY{ 100 };
	// amount to offest new platforms
	const short int yOffset{ 30 };
};

