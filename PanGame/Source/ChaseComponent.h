#pragma once
#include "ComponentList.h"
#include "Initializers.h"
#include "Definitions.h"


class ChaseComponent : public Component {
public:
	ChaseComponent(std::shared_ptr<GameObject> owner);
	~ChaseComponent();

	void Update() override;
	Vector2D seek(Vector2D targetPosition);  //returns desired velocity minus the current objectBody velocity
	Vector2D pursuit();   //calculates target's future position
	std::shared_ptr<BodyComponent> objectBody{ nullptr };  //pursuer
	std::shared_ptr<BodyComponent> targetBody{ nullptr };  //being pursued
	bool disabled{ false }; //needed to disable this component and get objectBody off screen
private:
	const int MAX_VELOCITY{ 2 };
	const int MAX_FORCE{ 10 };

};