#include "Engine.h"
#include "View.h"
#include <fstream>

Engine::Engine(std::string levelConfig, std::string libraryConfig)
{
	// Reads in high score
	std::ifstream fin;
	fin.open("./Assets/Config/highscore.txt");
	fin >> highScore;
	fin.close();

	//Configuring Assets
	tinyxml2::XMLDocument libraryDoc;
	if (libraryDoc.LoadFile(libraryConfig.c_str()) != tinyxml2::XML_SUCCESS)
	{
		printf("Bad Library File Path");
		exit(1);
	}

	tinyxml2::XMLElement* libraryRoot = libraryDoc.FirstChildElement("Library");  //Root
	tinyxml2::XMLElement* assetLibraryElement = libraryRoot->FirstChildElement("AssetLibrary"); // Asset Library

	//Configuring Game
	tinyxml2::XMLDocument levelDoc;
	if (levelDoc.LoadFile(levelConfig.c_str()) != tinyxml2::XML_SUCCESS)
	{
		printf("Bad Level File Path");
		exit(1);
	}

	tinyxml2::XMLElement* root = levelDoc.FirstChildElement("Game");
	tinyxml2::XMLElement* gameElement = root->FirstChildElement(); //Screen

	int screenWidth{ 0 };
	int screenHeight{ 0 };

	gameElement->QueryIntAttribute("width", &screenWidth);
	gameElement->QueryIntAttribute("height", &screenHeight);

	// irconde: Creating a GraphicsDevice

	gDevice = std::make_unique<GraphicsDevice>(screenWidth, screenHeight, true);

	//Construction Asset Library
	assetLibrary = (std::make_unique<Library>());
	tinyxml2::XMLElement* asset = assetLibraryElement->FirstChildElement("Asset");

	//checking if texture needs to be clipped
	bool isAnimated{ false };
	Vector2D libraryClipInfo{ 0.0f,0.0f };

	while (asset) {
		asset->QueryBoolAttribute("isAnimated", &isAnimated);
		asset->QueryFloatAttribute("height", &libraryClipInfo.y);
		asset->QueryFloatAttribute("width", &libraryClipInfo.x);

		//Adding assets to Library
		assetLibrary->addArtAsset(gDevice.get(), asset->Attribute("name"), asset->Attribute("spritepath"), isAnimated, libraryClipInfo);
		asset = asset->NextSiblingElement("Asset");
	}


	// initialize soundController
	soundController = std::make_unique<SoundController>();
	if (!soundController->initialize(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
		printf("SDL Mixer could not initialize! SDL_Error: %s\n", Mix_GetError());
		exit(1);
	}

	// get the sound library element
	tinyxml2::XMLElement* soundLibraryElement = assetLibraryElement->NextSiblingElement("SoundLibrary");
	asset = soundLibraryElement->FirstChildElement("Asset");
	while (asset) {
		if (!soundController->addSoundAsset(asset->Attribute("name"), asset->Attribute("soundPath"))) {
			printf("Failed to create sound asset in Engine\n");
			exit(1);
		}
		asset = asset->NextSiblingElement("Asset");
	}

	// get musicLibraryElement
	tinyxml2::XMLElement* musicLibraryElement = assetLibraryElement->NextSiblingElement("MusicLibrary");
	asset = musicLibraryElement->FirstChildElement("Asset");
	while (asset) {
		if (!soundController->addMusicAsset(asset->Attribute("name"), asset->Attribute("soundPath"))) {
			printf("Failed to create music asset in Engine\n");
			exit(1);
		}
		asset = asset->NextSiblingElement("Asset");
	}


	gameElement = gameElement->NextSiblingElement(); //FPS
	FPS = std::stoi(gameElement->GetText());

	// irconde: Creating an Inputdevice

	iDevice = std::make_unique<InputDevice>();


	// Initializing the Physics Device
	Vector2D gravity = { 0.0f,1000.0f };
	pDevice = std::make_unique<PhysicsDevice>(gravity);
	if (!pDevice->initialize(soundController.get())) {
		printf("Failed to initialize physics device");
		exit(1);
	}


	// irconde: Creating an ObjectFactory
	factory = std::make_unique<ObjectFactory>(gDevice.get(), assetLibrary.get(), iDevice.get(), pDevice.get());

	gameElement = gameElement->NextSiblingElement(); //Devices

	tinyxml2::XMLElement* deviceConfig = gameElement->FirstChildElement("Input");


	gameElement = gameElement->NextSiblingElement();  //Objects

	gameElement->FirstChildElement();

	tinyxml2::XMLElement* platformElement = gameElement;
	std::shared_ptr<BodyComponent> playerBodyComponent{ nullptr };
	std::shared_ptr<UserInputComponent> playerInputComponent{ nullptr };


	// irconde. Constructing objects
	for (tinyxml2::XMLElement* currentElement = gameElement; currentElement; currentElement = currentElement->NextSiblingElement()) {
		//constructing objects
		objects.push_back(std::shared_ptr<GameObject>(factory->create(currentElement)));
	}

	// search for the object with the userInputComponent which is the player
	for (auto& object : objects) {
		if (object->GetComponent<UserInputComponent>()) {
			playerBodyComponent = object->GetComponent<BodyComponent>();
			playerInputComponent = object->GetComponent<UserInputComponent>();
			break;
		}
	}

	if (!playerBodyComponent) {
		printf("Player body not found!");
		exit(1);
	}
	if (!playerInputComponent) {
		printf("Player input not found!");
		exit(1);
	}

	// create StaticHandler for dynamic platform creation and deletion
	staticHandler = std::make_unique<StaticHandler>(factory.get(), playerBodyComponent, playerInputComponent, soundController.get());

	soundController->playMusic(Sound::BACKGROUND_MUSIC);
}

Engine::~Engine()
{
	this->reset();
}

void Engine::reset()
{
	iDevice = nullptr;
	gDevice = nullptr;
	pDevice = nullptr;
	assetLibrary = nullptr;
	factory = nullptr;
}

bool Engine::run()
{
	// if player is dead, then write high score to file
	if (isDead && !gameOver) {
		writeHighScore();
		soundController->pauseMusic();
		soundController->playSound(Sound::GAME_OVER, 0);
		gameOver = true;
	}

	// if window is closed, write high score to file
	if (iDevice->GetEvent(InputDevice::gameEvent::QUIT)) {
		writeHighScore();
		return false;
	}

	std::unique_ptr<Timer>frameRate = std::make_unique<Timer>();
	if (!frameRate->Initialize(FPS)) {
		printf("Frame Timer could not intialize! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	frameRate->start();
	this->update();
	this->draw();
	frameRate->fpsRegulate();
	return true;
}

void Engine::update()
{
	iDevice->Update();
	pDevice->update(1.0f / 100.0f);



	for (auto& object : objects) {
		object->Update();
	}

	// grab new objects from staticHandler
	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> newObjects = staticHandler->update(objects);

	if (newObjects) {
		for (auto& newObject : *(newObjects)) {
			// insert in the second position after the backgound object
			objects.insert(objects.begin() + 1, newObject);
		}
	}


}

void Engine::draw()
{
	gDevice->Begin();
	for (auto& object : objects) {
		object->draw();
	}

	gDevice->update(highScore, score, isDead);
	gDevice->Present();
}

void Engine::writeHighScore() {

	// Outputs high score to file
	std::ofstream fout;
	fout.open("./Assets/Config/highscore.txt");
	fout << highScore;
	fout.close();
}
