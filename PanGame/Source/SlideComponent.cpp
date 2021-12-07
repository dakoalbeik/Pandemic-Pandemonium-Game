#include "SlideComponent.h"
#include "PhysicsDevice.h"

SlideComponent::SlideComponent(std::shared_ptr<GameObject> owner) : Component(owner) {

}

SlideComponent::~SlideComponent() {

}

void SlideComponent::Update() {

}

void SlideComponent::setSlide(std::shared_ptr<UserInputComponent> playerInputComponent) {



	GamePosition prevPostion{ objectBody->getPDevice()->getPosition(owner.get()), objectBody->getPDevice()->getAngle(owner.get()) };
	prevPostion.position.y += 3;

	objectBody->getPDevice()->setTransform(owner.get(), prevPostion);
}
