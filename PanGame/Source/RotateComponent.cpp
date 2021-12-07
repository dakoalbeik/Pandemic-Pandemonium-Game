#include "RotateComponent.h"
#include "PhysicsDevice.h"

RotateComponent::RotateComponent(std::shared_ptr<GameObject> owner) : Component(owner) {

}
RotateComponent::~RotateComponent() {}

void RotateComponent::Update() {

	// get current position and increment the angle
	GamePosition positionElements{ {objectBody->getPosition()}, objectBody->getAngle() };
	positionElements.angle++;

	if (positionElements.angle >= 360.0f) {
		positionElements.angle = 0.0f;
	}


	objectBody->getPDevice()->setTransform(owner.get(), positionElements);
}

