#include "GameObject.h"
#include "ComponentList.h"
GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::AddComponent(std::shared_ptr<Component>component)
{
	components.push_back(component);
}

void GameObject::Update()
{
	for (auto& component : components) {
		component->Update();
	}
}

void GameObject::draw()
{
	std::shared_ptr<SpriteComponent> sprite = GetComponent<SpriteComponent>();
	if (sprite != nullptr) {
		sprite->draw();
	}
}
