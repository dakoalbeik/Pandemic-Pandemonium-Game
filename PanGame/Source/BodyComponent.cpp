#include "BodyComponent.h"
#include "PhysicsDevice.h"
#include <iostream>

BodyComponent::BodyComponent(std::shared_ptr<GameObject> owner, BodyPresets& presets, PhysicsDevice* pDevice) : Component(owner), pDevice(pDevice) {
	objectType = presets.objectType;

	if (!pDevice->createFixture(owner.get(), presets.physics, presets.positionElements)) {
		printf("Failed to create fixture!");
		exit(1);
	};

}
BodyComponent::~BodyComponent()
{
}

void BodyComponent::Update() {
	if (getObjectType() == ObjectType::Player
		&& owner->GetComponent<BodyComponent>()->getPosition().y > SCREEN_HEIGHT) {
		isDead = true;
	}
	else if (getObjectType() != ObjectType::Player &&
		owner->GetComponent<BodyComponent>()->getPosition().y > SCREEN_HEIGHT) {
		dead = true;
	}
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