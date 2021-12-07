#pragma once
#include "ComponentList.h"
#include "GameObject.h"
#include "SDL.h"
#include "tinyxml2.h"
#include "Initializers.h"
#include "Texture.h"


#include "Definitions.h"
class RotateComponent : public Component {
public:
	RotateComponent(std::shared_ptr<GameObject> owner);
	~RotateComponent();

	void Update();
	std::shared_ptr<BodyComponent> objectBody{ nullptr };

private:

};