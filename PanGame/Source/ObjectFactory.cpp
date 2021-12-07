#include "ObjectFactory.h"
#include "PhysicsDevice.h"
#include <iostream>
#include <string>

std::shared_ptr<GameObject> ObjectFactory::create(tinyxml2::XMLElement* objectElement)
{
	std::shared_ptr<ObjectFactoryPresets> presets = createPresetsFromXML(objectElement);
	std::shared_ptr<GameObject> newObject = create(presets.get());
	return newObject;
}

std::shared_ptr<GameObject> ObjectFactory::create(ObjectFactoryPresets* presets)
{
	//add components to each object if it exists in the xml
	std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>();
	if (presets->inputInitializers.createUserInput) {
		newObject->AddComponent(std::make_shared<UserInputComponent>(newObject, iDevice));
	}
	if (presets->chaseInitializers.createChase) {
		newObject->AddComponent(std::make_shared<ChaseComponent>(newObject));
	}
	if (presets->spriteInitializers.createSprite) {
		newObject->AddComponent(std::make_shared<SpriteComponent>(newObject, gDevice, presets->spriteInitializers));
	}
	if (presets->bodyInitializers.createBody) {
		newObject->AddComponent(std::make_shared<BodyComponent>(newObject, presets->bodyInitializers, pDevice));
		newObject->GetComponent<SpriteComponent>()->spriteBody = newObject->GetComponent<BodyComponent>();
		if (auto chaseComponent = newObject->GetComponent<ChaseComponent>()) {
			chaseComponent->objectBody = newObject->GetComponent<BodyComponent>();
		}
	}
	if (presets->moveInitializers.createMove) {
		newObject->AddComponent(std::make_shared<MoveComponent>(newObject));
		newObject->GetComponent<MoveComponent>()->objectBody = newObject->GetComponent<BodyComponent>();
		// lock the rotation on the object with a move component (player object)
		newObject->GetComponent<MoveComponent>()->objectBody->getPDevice()->setFixedRotation(newObject.get(), true);
	}
	if (presets->rotateInitializers.createRotate) {
		newObject->AddComponent(std::make_shared<RotateComponent>(newObject));
		newObject->GetComponent<RotateComponent>()->objectBody = newObject->GetComponent<BodyComponent>();
	}
	if (presets->slideInitializers.createSlide) {
		newObject->AddComponent(std::make_shared<SlideComponent>(newObject));
		newObject->GetComponent<SlideComponent>()->objectBody = newObject->GetComponent<BodyComponent>();
	}
	// Use presents to create new object with the desired components and initialize their properties


	return newObject;
}

std::shared_ptr<ObjectFactoryPresets> ObjectFactory::createPresetsFromXML(tinyxml2::XMLElement* objectElement)
{
	//populate presets for each object if component exists in xml
	std::unique_ptr<ObjectFactoryPresets> presets = std::make_unique<ObjectFactoryPresets>();
	for (tinyxml2::XMLElement* componentElement = objectElement->FirstChildElement(); componentElement; componentElement = componentElement->NextSiblingElement()) {
		std::string componentName = componentElement->Attribute("name");
		if (componentName == "Sprite") {
			addSpritePresets(&presets->spriteInitializers, componentElement);
		}
		else if (componentName == "Body") {
			addBodyPresets(&presets->bodyInitializers, componentElement);
		}
		else if (componentName == "UserInput") {
			addInputPresets(&presets->inputInitializers, componentElement);
		}
		else if (componentName == "Move") {
			addMovePresets(&presets->moveInitializers, componentElement);
		}
		else if (componentName == "Rotate") {
			addRotatePresets(&presets->rotateInitializers, componentElement);
		}
		else if (componentName == "Slide") {
			addSlidePresets(&presets->slideInitializers, componentElement);
		}
		else if (componentName == "Chase") {
			addChasePresets(&presets->chaseInitializers, componentElement);
		}
		// Load presents from xml related to other components here 
	}
	return presets;
}

//methods to add presets to objects
void ObjectFactory::addSpritePresets(SpritePresets* presets, tinyxml2::XMLElement* componentElement)
{
	presets->createSprite = true;
	presets->spriteTexture = assetLibrary->getArtAsset(componentElement->Attribute("asset"));

	componentElement->QueryBoolAttribute("isAnimated", &presets->isAnimated);

	if (presets->isAnimated) {
		componentElement = componentElement->FirstChildElement();
		presets->clip = std::make_shared<ClipInfo>();
		componentElement->QueryIntAttribute("spriteID", &presets->clip->spriteID);
		componentElement->QueryIntAttribute("animLag", &presets->clip->animLag);
		componentElement->QueryIntAttribute("numSprites", &presets->clip->numSprites);
		componentElement->QueryIntAttribute("height", &presets->clip->height);
		componentElement->QueryIntAttribute("width", &presets->clip->width);
		componentElement->QueryIntAttribute("col_num", &presets->clip->col_num);
		componentElement->QueryIntAttribute("row_num", &presets->clip->row_num);
	}

}
void ObjectFactory::addBodyPresets(BodyPresets* presets, tinyxml2::XMLElement* componentElement)
{
	presets->createBody = true;
	componentElement->QueryFloatAttribute("x", (float*)(&presets->positionElements.position.x));
	componentElement->QueryFloatAttribute("y", (float*)(&presets->positionElements.position.y));
	componentElement->QueryFloatAttribute("angle", (float*)(&presets->positionElements.angle));


	// initializing body type
	std::string tempAttribute = componentElement->Attribute("objectType");
	if (tempAttribute == "Player") {
		presets->objectType = ObjectType::Player;
	}
	else if (tempAttribute == "Virus") {
		presets->objectType = ObjectType::Virus;
	}
	else if (tempAttribute == "Floor") {
		presets->objectType = ObjectType::Floor;
	}
	else if (tempAttribute == "Item") {
		presets->objectType = ObjectType::Item;
	}
	else if (tempAttribute == "Karen") {
		presets->objectType = ObjectType::Karen;
	}
	else if (tempAttribute == "Platform") {
		presets->objectType = ObjectType::Platform;
	}
	else if (tempAttribute == "Any") {
		presets->objectType = ObjectType::Any;
	}

	// terminate program if no physics attributes are found
	componentElement = componentElement->FirstChildElement();
	if (!componentElement) {
		printf("No physics component found.");
		exit(1);
	}

	componentElement->QueryFloatAttribute("spinSpeed", (float*)(&presets->physics.spinSpeed));

	// initializing physics body type
	tempAttribute = componentElement->Attribute("physicsBodyType");
	if (tempAttribute == "Static") {
		presets->physics.physicsBodyType = GAME_STATIC;
	}
	else if (tempAttribute == "Dynamic") {
		presets->physics.physicsBodyType = GAME_DYNAMIC;
	}

	// initializing object shape
	tempAttribute = componentElement->Attribute("objectShape");
	if (tempAttribute == "Rectangle") {
		presets->physics.objectShape = GAME_RECTANGLE;
	}
	else if (tempAttribute == "Circle") {
		presets->physics.objectShape = GAME_CIRCLE;
	}

	componentElement->QueryFloatAttribute("density", (float*)(&presets->physics.density));
	componentElement->QueryFloatAttribute("friction", (float*)(&presets->physics.friction));
	componentElement->QueryFloatAttribute("restitution", (float*)(&presets->physics.restitution));
	componentElement->QueryFloatAttribute("angularDamping", (float*)(&presets->physics.angularDamping));
	componentElement->QueryFloatAttribute("linearDamping", (float*)(&presets->physics.linearDamping));
	componentElement->QueryFloatAttribute("force", (float*)(&presets->physics.force));
	componentElement->QueryBoolAttribute("physicsOn", &presets->physics.physicsOn);


}
void ObjectFactory::addInputPresets(UserInputPresets* presets, tinyxml2::XMLElement* componentElement) {
	presets->createUserInput = true;
}
void ObjectFactory::addMovePresets(MovePresets* presets, tinyxml2::XMLElement* componentElement) {
	presets->createMove = true;
}
void ObjectFactory::addRotatePresets(RotatePresets* presets, tinyxml2::XMLElement* componentElement) {
	presets->createRotate = true;
}
void ObjectFactory::addSlidePresets(SlidePresets* presets, tinyxml2::XMLElement* componentElement) {
	presets->createSlide = true;
}
void ObjectFactory::addChasePresets(ChasePresets* presets, tinyxml2::XMLElement* componentElement) {
	presets->createChase = true;
}



// Methods to load component-specific presents from the xml file
