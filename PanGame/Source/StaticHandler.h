#pragma once
#include "Definitions.h"
#include "tinyxml2.h"
#include "TimeInterval.h"
#include <memory>
#include <vector>


class StaticHandler
{
public:
	StaticHandler(ObjectFactory* factory, std::shared_ptr<BodyComponent> playerBodyComponent,
		std::shared_ptr<UserInputComponent> playerInputComponent, SoundController* soundController);
	~StaticHandler();

	// gets called by the engine. Adds and removes platforms
	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> update(std::vector<std::shared_ptr<GameObject>>& gameObjects);


private:
	std::shared_ptr<BodyComponent> playerBodyComponent{ nullptr };
	std::shared_ptr<UserInputComponent> playerInputComponent{ nullptr };
	std::unique_ptr<RandomPosition> randomHandler{ nullptr };
	std::shared_ptr<BodyComponent> lastPlatformBody{ nullptr };

	SoundController* soundController{ nullptr };
	std::unique_ptr<TimeInterval> time{ nullptr };

	ObjectFactory* factory{ nullptr };
	tinyxml2::XMLElement* platformElement{ nullptr };
	tinyxml2::XMLElement* karenElement{ nullptr };
	tinyxml2::XMLDocument futureDoc;

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> createObjects();
	void physicsEnabled(std::vector<std::shared_ptr<GameObject>>& gameObjects);
	void createPlatforms(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects);
	void handleKarenCreation(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects);
	void slideObjects(std::vector<std::shared_ptr<GameObject>>& gameObjects);

	void getKarenOffScreen(std::vector<std::shared_ptr<GameObject>>& newObjects);

	//flags for creation or killing
	bool firstRound{ true };
	bool createKaren{ true };
};

