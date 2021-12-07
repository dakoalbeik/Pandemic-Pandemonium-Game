#pragma once

#include "Component.h"
#include "ComponentList.h"
#include "Vector2D.h"
#include "Initializers.h"
#include "Definitions.h"

class MoveComponent : public Component {
public:
	MoveComponent(std::shared_ptr<GameObject> owner);
	~MoveComponent();
	void Update() override;
	void Jump();

	std::shared_ptr<BodyComponent> objectBody{ nullptr };
private:
	std::shared_ptr<UserInputComponent> inputComponent{ nullptr };
	//helper functions to change body component's position
	void MoveRight();
	void MoveLeft();
	void offScreen();
	void Move();
};

