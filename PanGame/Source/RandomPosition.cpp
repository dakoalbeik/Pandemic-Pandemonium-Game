#include "RandomPosition.h"

std::random_device RandomPosition::seed;
std::default_random_engine RandomPosition::e(seed());

RandomPosition::RandomPosition() {

}

RandomPosition::~RandomPosition() {

}

Vector2D RandomPosition::getRandPosition(int currentY) {

	// make currentY positive
	currentY *= -1;

	// return one random position offsetting the y based on the previous y
	std::uniform_int_distribution<int> randomX(10, 500);
	std::uniform_int_distribution<int> randomY(currentY, currentY + maxDistanceY);
	//set y negative again so it appears above the screen
	Vector2D temp{ (float)randomX(e), -(float)randomY(e) };

	return temp;
}

std::vector<Vector2D>& RandomPosition::getRandomPositions() {
	// erase previous positions in vector
	positions.clear();

	int currentY{ -yOffset };
	Vector2D tempPosition{ 0,0 };

	// keep generating positions until we populate an entire screen full
	while (tempPosition.y > maxY) {
		tempPosition = getRandPosition(currentY);
		positions.push_back(tempPosition);
		currentY = tempPosition.y - yOffset;
	}

	return positions;
}


