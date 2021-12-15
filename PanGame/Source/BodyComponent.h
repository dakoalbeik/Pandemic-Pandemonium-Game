#pragma once

#include "Component.h"
#include "ComponentList.h"
#include "Vector2D.h"
#include "Initializers.h"
#include "Definitions.h"


class BodyComponent : public Component {
public:
	BodyComponent(std::shared_ptr<GameObject> owner, BodyPresets& presets, PhysicsDevice* pDevice);
	~BodyComponent();

	void Update() override;

	float getAngle();
	Vector2D getPosition();
	Vector2D getVelocity();
	ObjectType getObjectType();
	Item getItemType();
	std::shared_ptr<PhysicsDevice> getPDevice();

	bool dead{ false };
private:
	std::shared_ptr<PhysicsDevice> pDevice;
	ObjectType objectType{ ObjectType::Any };
	Item itemType{ Item::NONE };
	void setDead();
};

