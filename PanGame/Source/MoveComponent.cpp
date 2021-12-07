#include "MoveComponent.h"
#include "PhysicsDevice.h"
#include <iostream>

MoveComponent::MoveComponent(std::shared_ptr<GameObject> owner) : Component(owner) {
	inputComponent = owner->GetComponent<UserInputComponent>();
}
MoveComponent::~MoveComponent() {}

void MoveComponent::Update() {
	if (inputComponent) {
		Move();   //calls appropriate move method based on key pressed
		offScreen();  //adjust players position if he travels offscreen
	}
}

void MoveComponent::MoveRight() {
	// apply linear x velocity and use the same y velocity
	float y = objectBody->getPDevice()->getVelocity(owner.get()).y;
	objectBody->getPDevice()->setLinearVelocity(owner.get(), { 300, y });

}
void MoveComponent::MoveLeft() {
	// apply negative linear x velocity and use the same y velocity
	float y = objectBody->getPDevice()->getVelocity(owner.get()).y;
	objectBody->getPDevice()->setLinearVelocity(owner.get(), { -300, y });
}
void MoveComponent::Jump() {
	float x = objectBody->getPDevice()->getVelocity(owner.get()).x;
	objectBody->getPDevice()->setLinearVelocity(owner.get(), { x, -1100 });
}


void MoveComponent::offScreen() {
	//Get player to run off screen and back on the other side
	auto [x, y] = objectBody->getPosition();
	float spriteWidth = owner.get()->GetComponent<SpriteComponent>()->getTexture()->getWidth();
	if (x > SCREEN_WIDTH) {
		objectBody->getPDevice()->setTransform(owner.get(), { { 0 - spriteWidth, y },0 });
	}
	else if (x + spriteWidth < 0) {

		objectBody->getPDevice()->setTransform(owner.get(), { { SCREEN_WIDTH, y },0 });
	}
}

void MoveComponent::Move() {

	if (inputComponent->getInputDevice()->GetEvent(InputDevice::gameEvent::LEFT)) {
		MoveLeft();
	}
	if (inputComponent->getInputDevice()->GetEvent(InputDevice::gameEvent::RIGHT)) {
		MoveRight();
	}
}