#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
#include <string>
#include <memory>
#include <SDL.h>

#include "Definitions.h"
#include "Library.h"
#include "ObjectFactory.h"
#include "InputDevice.h"
#include "PhysicsDevice.h"
#include "StaticHandler.h"
#include "SoundController.h"
#include "Timer.h"

class Engine {
public:
	Engine(std::string levelConfig, std::string libraryConfig);
	~Engine();
	void reset();
	bool run();
	void update();
	void draw();


private:

	void writeHighScore();
	//Devices
	std::unique_ptr<GraphicsDevice> gDevice{ nullptr };
	std::unique_ptr<InputDevice> iDevice{ nullptr };
	std::unique_ptr<PhysicsDevice> pDevice{ nullptr };
	std::unique_ptr<StaticHandler> staticHandler{ nullptr };
	std::unique_ptr<SoundController> soundController{ nullptr };

	//Library
	std::unique_ptr<Library> assetLibrary{ nullptr };

	//factory
	std::unique_ptr<ObjectFactory> factory{ nullptr };

	std::vector<std::shared_ptr<GameObject>> objects;

	Uint32 FPS{ 0 };

	bool gameOver{ false };

};

#endif // !ENGINE_H

