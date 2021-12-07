#include <string>
#include <memory>
#include "Engine.h"

int main(int argc, char* argv[]) {
	std::string levelConfig = "./Assets/Config/LevelConfig.xml";
	std::string libraryConfig = "./Assets/Config/LibraryConfig.xml";
	std::unique_ptr<Engine> engine{ std::make_unique<Engine>(levelConfig, libraryConfig) };
	while (engine->run()) {}

	engine = nullptr;
	return 0;
}