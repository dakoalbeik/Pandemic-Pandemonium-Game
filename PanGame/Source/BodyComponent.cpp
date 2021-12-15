#include "BodyComponent.h"
#include "PhysicsDevice.h"
#include <iostream>

BodyComponent::BodyComponent(std::shared_ptr<GameObject> owner, BodyPresets& presets, PhysicsDevice* pDevice) : Component(owner), pDevice(pDevice) {
	objectType = presets.objectType;
	itemType = presets.itemType;

	if (!pDevice->createFixture(owner.get(), presets.physics, presets.positionElements)) {
		printf("Failed to create fixture!");
		exit(1);
	};

}

BodyComponent::~BodyComponent()
{
}

void BodyComponent::Update() {
	if (getObjectType() == ObjectType::Player && getPosition().y > SCREEN_HEIGHT) {
		isPlayerDead = true;
	}

	setDead();
}

float BodyComponent::getAngle()
{
	return pDevice->getAngle(owner.get());
}

Vector2D BodyComponent::getPosition()
{
	return pDevice->getPosition(owner.get());
}

std::shared_ptr<PhysicsDevice> BodyComponent::getPDevice() {
	return pDevice;
}

Vector2D BodyComponent::getVelocity() {
	return pDevice->getVelocity(owner.get());
}

ObjectType BodyComponent::getObjectType() {
	return objectType;
}

Item BodyComponent::getItemType() {
	return itemType;
}

void BodyComponent::setDead() {

	if (getObjectType() != ObjectType::Player && getPosition().y > SCREEN_HEIGHT + 30) {
		dead = true;
		if (getObjectType() == ObjectType::Platform) {
			score++;
		}
	}

	if (highScore < score) {
		highScore = score;
	}
}