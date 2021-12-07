#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H
#include <vector>
#include <memory>
#include "Definitions.h"

class Component {
public:
	Component(std::shared_ptr<GameObject> owner);
	~Component();

	std::shared_ptr<GameObject> getOwner() const;
	virtual void Update() = 0;
protected:
	std::shared_ptr<GameObject> owner{ nullptr };
};
#endif // !COMPONENT_H
