#pragma once
#include "Vector2D.h"
#include <vector>
#include <random>


class RandomPosition {
public:
	static std::random_device seed;
	static std::default_random_engine e;
	RandomPosition();
	~RandomPosition();


	// returns a vector of random positions
	std::vector<Vector2D>& getRandomPositions();
private:

	// stores the random positions 
	std::vector<Vector2D> positions;

	// return random vector based on previous Y
	Vector2D getRandPosition(int currentY);

	// highest y position for new platforms
	const int maxY{ -600 };
	// max disntace between platforms
	const int maxDistanceY{ 100 };
	// amount to offest new platforms
	const short int yOffset{ 30 };
};

