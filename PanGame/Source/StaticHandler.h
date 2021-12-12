#pragma once
#include "Definitions.h"
#include "tinyxml2.h"
#include <memory>
#include <vector>
#include <ctime>
#include <sstream>


class StaticHandler
{
public:
	StaticHandler(ObjectFactory* factory, std::shared_ptr<BodyComponent> playerBodyComponent,
		std::shared_ptr<UserInputComponent> playerInputComponent, SoundController* soundController);
	~StaticHandler();

	// gets called by the engine. Adds and removes platforms
	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> update(std::vector<std::shared_ptr<GameObject>>& gameObjects);

	//timer for karens arrival and departure
	time_t now{ time(NULL) };
	// tm is a time struct with several members
	tm karenInitial;
	tm start;
private:
	std::shared_ptr<BodyComponent> playerBodyComponent{ nullptr };
	std::shared_ptr<UserInputComponent> playerInputComponent{ nullptr };
	std::unique_ptr<RandomPosition> randomHandler{ nullptr };
	std::shared_ptr<BodyComponent> lastPlatformBody{ nullptr };
	std::shared_ptr<BodyComponent> karenBody{ nullptr };

	SoundController* soundController{ nullptr };

	ObjectFactory* factory{ nullptr };
	tinyxml2::XMLElement* platformElement{ nullptr };
	tinyxml2::XMLElement* karenElement{ nullptr };
	tinyxml2::XMLDocument futureDoc;

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> createObjects();
	void deleteObjects(std::vector<std::shared_ptr<GameObject>>& gameObjects);
	void physicsEnabled(std::vector<std::shared_ptr<GameObject>>& gameObjects);
	void slideObjects(std::vector<std::shared_ptr<GameObject>>& gameObjects);

	void getKarenOffScreen(std::vector<std::shared_ptr<GameObject>>& newObjects);
	void removeKaren(std::vector<std::shared_ptr<GameObject>>& newObjects);

	//flags for creation or killing
	bool firstRound{ true };
	bool killKaren{ false };
	bool createKaren{ true };
};

