#ifndef LIBRARY_H
#define LIBRARY_H


#include <string>
#include <map>
#include "Texture.h"
#include "GraphicsDevice.h"
#include "ObjectFactory.h"
#include "Definitions.h"

class Library {

public:
	std::map<std::string, std::unique_ptr<ObjectFactory>> library;
	std::map<std::string, std::shared_ptr<Texture>> artLibrary;

	Library();
	~Library();

	std::shared_ptr<Texture> getArtAsset(std::string keyName);
	bool addArtAsset(GraphicsDevice* gDevice, std::string keyName, std::string spritePath, bool isAnimated, Vector2D clipInfo);

};

#endif // !LIBRARY_H