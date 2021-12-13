#include "StaticHandler.h"
#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include "RandomHandler.h"
#include "SoundController.h"
#include <string>
#include <random>


StaticHandler::StaticHandler(ObjectFactory* factory, std::shared_ptr<BodyComponent> playerBodyComponent,
	std::shared_ptr<UserInputComponent> playerInputComponent, SoundController* soundController) :
	playerBodyComponent(playerBodyComponent), playerInputComponent(playerInputComponent), factory(factory), soundController(soundController)
{
	time = std::make_unique<TimeInterval>();

	//Configuring Future Assets
	if (futureDoc.LoadFile("./Assets/Config/FutureObjects.xml") != tinyxml2::XML_SUCCESS)
	{
		printf("Bad Library File Path");
		exit(1);
	}

	tinyxml2::XMLElement* FutureRoot = futureDoc.FirstChildElement("Objects");  //Root

	//Grabbing Karen info and platform info from xml to use to pass into factory create method later
	tinyxml2::XMLElement* enemyElement = FutureRoot->FirstChildElement("Enemy");
	tinyxml2::XMLElement* platformsElement = enemyElement->NextSiblingElement("Platforms");

	//searching for karen in xml
	for (karenElement = enemyElement->FirstChildElement(); karenElement; karenElement = karenElement->NextSiblingElement()) {
		std::string componentName = karenElement->Attribute("name");
		if (componentName == "Karen") {
			break;
		}
	}

	if (!karenElement) {
		printf("Failed to grab Karen from xml!");
		exit(1);
	}

	//searching for platform in xml
	for (platformElement = platformsElement->FirstChildElement(); platformElement; platformElement = platformElement->NextSiblingElement()) {
		std::string componentName = platformElement->Attribute("name");
		if (componentName == "Platform") {
			break;
		}
	}

	if (!platformElement) {
		printf("Failed to grab Platform from xml!");
		exit(1);
	}

	// store XML Items in vector
	tinyxml2::XMLElement* platformItems = platformsElement->NextSiblingElement("Items");
	tinyxml2::XMLElement* item = platformItems->FirstChildElement("Object");
	float width{ 0 };
	float height{ 0 };
	ItemPreset tempPreset{ nullptr, {0,0} };
	while (item) {
		// query sprite width and height
		item->QueryFloatAttribute("width", &width);
		item->QueryFloatAttribute("height", &height);
		tempPreset = { item, {width, height} };
		platformItemsXML.push_back(tempPreset);
		item = item->NextSiblingElement();
	}

	randomHandler = std::make_unique<RandomHandler>();

}

StaticHandler::~StaticHandler() {

}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> StaticHandler::update(std::vector<std::shared_ptr<GameObject>>& gameObjects) {


	getKarenOffScreen(gameObjects);

	//decide whether to enable physics on static objects
	physicsEnabled(gameObjects);

	// check player's y postion, decide to scroll platfroms
	if (playerBodyComponent->getPosition().y <= SCREEN_HEIGHT / 2) {
		slideObjects(gameObjects);
	}

	return createObjects();
}

void StaticHandler::slideObjects(std::vector<std::shared_ptr<GameObject>>& gameObjects) {

	for (auto& object : gameObjects) {
		if (auto platformComponent = object->GetComponent<SlideComponent>()) {
			platformComponent->setSlide(playerInputComponent);
		}
	}

}

std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> StaticHandler::createObjects() {

	// make an empty vector of objects
	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects = std::make_shared<std::vector<std::shared_ptr<GameObject>>>();

	handleKarenCreation(newObjects);

	createPlatforms(newObjects);

	return newObjects;
}

void StaticHandler::physicsEnabled(std::vector<std::shared_ptr<GameObject>>& gameObjects) {
	for (auto& object : gameObjects) {


		if (auto platformComponent = object->GetComponent<SlideComponent>(); platformComponent) {
			if (platformComponent->objectBody->getPosition().y > playerBodyComponent->getPosition().y
				+ playerBodyComponent->getOwner()->GetComponent<SpriteComponent>()->getTexture()->getHeight() - 3) {
				platformComponent->objectBody->getPDevice()->setEnabled(object.get(), true);
			}
			else {
				platformComponent->objectBody->getPDevice()->setEnabled(object.get(), false);
			}
		}

	}
}

void StaticHandler::createPlatforms(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects)
{
	// if last platform created is on screen, make some more
	if ((firstRound) || (lastPlatformBody->getPosition().y > 0)) {
		firstRound = false;
		// getting positions from randomHandler
		std::vector<PlatformPreset> platformPresets{ randomHandler->getRandomPositions() };

		// create a platform object for each position
		for (auto& platformPreset : platformPresets) {
			// create new vector with platforms
			newObjects->push_back(std::shared_ptr<GameObject>(factory->create(platformElement)));

			//find the body for the object and give it a random position
			std::shared_ptr<BodyComponent> tempBody = newObjects->back()->GetComponent<BodyComponent>();
			tempBody->getPDevice()->setTransform(newObjects->back().get(), { platformPreset.platformPosition, 0.0f });
			// ignoring the collsion with new platfroms
			tempBody->getPDevice()->setEnabled(newObjects->back().get(), false);

			// store the last platform's body position to decide when to create more platfroms
			lastPlatformBody = newObjects->back()->GetComponent<BodyComponent>();

			if (platformPreset.hasItem) {
				createItem(newObjects, platformPreset.itemName, platformPreset.platformPosition);
			}

		}


	}
}

void StaticHandler::handleKarenCreation(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects)
{
	//if karen flag for creation is true and it has been 5 seconds
	if (createKaren && !isPlayerDead && time->hasITBeen(5)) {
		//pause background music and play karen chase music
		soundController->pauseMusic();
		soundController->playSound(Sound::BOSS_MUSIC, 0);

		//create karen object on vector
		newObjects->push_back(std::shared_ptr<GameObject>(factory->create(karenElement)));
		newObjects->back()->GetComponent<ChaseComponent>()->targetBody = playerBodyComponent;
		newObjects->back()->GetComponent<BodyComponent>()->getPDevice()->setFixedRotation(newObjects->back().get(), true);

		//set karen flag to false so she can be deleted later
		createKaren = false;

		//play speak to the manager sound
		soundController->playSound(Sound::KAREN, 1);
	}

}

void StaticHandler::getKarenOffScreen(std::vector<std::shared_ptr<GameObject>>& newObjects)
{
	// get karen off screen after 8 seconds
	if (!createKaren && time->hasITBeen(8)) {
		createKaren = true;
		for (auto& object : newObjects) {
			if (auto karenBodyComponent = object->GetComponent<BodyComponent>(); karenBodyComponent->getObjectType() == ObjectType::Karen) {
				// disable chase behavior
				object->GetComponent<ChaseComponent>()->disabled = true;
				// increase karen's velocity
				karenBodyComponent->getPDevice()->setLinearVelocity(object.get(), { 400, 50 });
			}
		}

		//continue playing background music that was paused
		if (!isPlayerDead) {
			soundController->resumeMusic(Sound::BACKGROUND_MUSIC);
		}
	}
}

void StaticHandler::createItem(std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects, Item itemName, Vector2D platformPosition) {
	// create item based on the enum class passed in
	newObjects->push_back(std::shared_ptr<GameObject>(factory->create(platformItemsXML.at(static_cast<int>(itemName)).itemXML)));
	// change item position to be on top of the platfroms
	newObjects->back()->GetComponent<BodyComponent>()->getPDevice()->setTransform(newObjects->back().get(), { centerItem(platformPosition, itemName), 0 });

}

Vector2D StaticHandler::centerItem(Vector2D platformPosition, Item itemName) {

	const Vector2D platformDimensions{ 90, 24 };

	Vector2D itemPos{ 0, 0 };

	float platformHalf = platformPosition.x + (platformDimensions.x / 2);
	itemPos.x = platformHalf - (platformItemsXML.at(static_cast<int>(itemName)).dimensions.x / 2);
	itemPos.y = platformPosition.y - platformItemsXML.at(static_cast<int>(itemName)).dimensions.y;


	return itemPos;
}