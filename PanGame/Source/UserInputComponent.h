#pragma once

#include "Component.h"
#include "ComponentList.h"
#include "Vector2D.h"
#include "Initializers.h"
#include "Definitions.h"

class UserInputComponent : public Component {
public:
	//only used to provide the input device instance to other components
	UserInputComponent(std::shared_ptr<GameObject> owner, InputDevice* inputDevice);
	~UserInputComponent();
	void Update() override;
	InputDevice* getInputDevice();
private:
	InputDevice* iDevice{ nullptr };
};

