#pragma once
#include "ComponentList.h"
#include "Definitions.h"
#include "Initializers.h"

class SlideComponent : public Component {
public:
	SlideComponent(std::shared_ptr<GameObject> owner);
	~SlideComponent();

	void Update() override;

	void setSlide(std::shared_ptr<UserInputComponent> playerInputComponent);

	std::shared_ptr<BodyComponent> objectBody{ nullptr };
private:

};