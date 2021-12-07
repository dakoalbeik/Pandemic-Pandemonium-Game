#include "Component.h"

Component::Component(std::shared_ptr<GameObject> owner) : owner(owner)
{
}

Component::~Component()
{
}

std::shared_ptr<GameObject> Component::getOwner() const
{
	return(owner);
}
