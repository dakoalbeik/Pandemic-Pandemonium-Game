#include "RandomHandler.h"
#include <iostream>

std::random_device RandomHandler::seed;
std::default_random_engine RandomHandler::e(seed());

RandomHandler::RandomHandler() {

}

RandomHandler::~RandomHandler() {

}

Vector2D RandomHandler::getRandPosition(int currentY) {

	// make currentY positive
	currentY *= -1;

	// return one random position offsetting the y based on the previous y
	std::uniform_int_distribution<int> randomX(10, 500);
	std::uniform_int_distribution<int> randomY(currentY, currentY + maxDistanceY);
	//set y negative again so it appears above the screen
	Vector2D temp{ (float)randomX(e), -(float)randomY(e) };

	return temp;
}

std::vector<PlatformPreset>& RandomHandler::getRandomPositions() {
	// erase previous positions in vector
	positions.clear();

	int currentY{ -yOffset };

	PlatformPreset tempPreset;


	// keep generating positions until we populate an entire screen full
	while (tempPreset.platformPosition.y > maxY) {
		tempPreset.platformPosition = getRandPosition(currentY);
		tempPreset.hasItem = false;
		if (shouldCreateItem(10.0f)) {  //10% chance of creation
			tempPreset.hasItem = true;
			tempPreset.itemName = randomItem(6);
		}
		positions.push_back(tempPreset);
		currentY = tempPreset.platformPosition.y - yOffset;
	}

	return positions;
}

bool RandomHandler::shouldCreateItem(float percent) {

	std::bernoulli_distribution shouldCreate(percent / 100);

	return shouldCreate(e);
}

Item RandomHandler::randomItem(const int maxRange)
{
	std::uniform_int_distribution<int> itemIndex(0, maxRange);

	return static_cast<Item>(itemIndex(e));
}