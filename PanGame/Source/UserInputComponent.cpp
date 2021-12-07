#include "UserInputComponent.h"

UserInputComponent::UserInputComponent(std::shared_ptr<GameObject> owner, InputDevice* inputDevice) : Component(owner) {
	iDevice = inputDevice;
}

UserInputComponent::~UserInputComponent() {}

void UserInputComponent::Update() {

}

InputDevice* UserInputComponent::getInputDevice() {
	return iDevice;
}
