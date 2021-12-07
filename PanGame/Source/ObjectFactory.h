#pragma once
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "tinyxml2.h"
#include "Initializers.h"
#include "GraphicsDevice.h"
#include "ComponentList.h"
#include "Library.h"
#include "Definitions.h"

class ObjectFactory {
public:
	ObjectFactory() {};
	//added iDevice to pass to constructor
	ObjectFactory(GraphicsDevice* gDevice, Library* assetLibrary, InputDevice* iDevice, PhysicsDevice* pDevice) :
		gDevice(gDevice), assetLibrary(assetLibrary), iDevice(iDevice), pDevice(pDevice) {};

	std::shared_ptr<GameObject> create(tinyxml2::XMLElement* objectElement);
	std::shared_ptr<GameObject> create(ObjectFactoryPresets* presets);
private:
	GraphicsDevice* gDevice{ nullptr };
	Library* assetLibrary{ nullptr };
	InputDevice* iDevice{ nullptr };
	PhysicsDevice* pDevice{ nullptr };

	std::shared_ptr<ObjectFactoryPresets> createPresetsFromXML(tinyxml2::XMLElement* objectElement);
	void addSpritePresets(SpritePresets* presets, tinyxml2::XMLElement* componentElement);
	void addBodyPresets(BodyPresets* presets, tinyxml2::XMLElement* componentElement);
	//added methods to add specific presets to an object
	void addInputPresets(UserInputPresets* presets, tinyxml2::XMLElement* componentElement);
	void addMovePresets(MovePresets* presets, tinyxml2::XMLElement* componentElement);
	void addRotatePresets(RotatePresets* presets, tinyxml2::XMLElement* componentElement);
	void addSlidePresets(SlidePresets* presets, tinyxml2::XMLElement* componentElement);
	void addChasePresets(ChasePresets* presets, tinyxml2::XMLElement* componentElement);
};
#endif // !OBJECTFACTORY_H
