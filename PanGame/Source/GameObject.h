#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <memory>
#include <SDL.h>
#include <vector>
#include "ComponentList.h"
#include "Definitions.h"
class GameObject
{

public:
	GameObject();
	~GameObject();

	void AddComponent(std::shared_ptr<Component> component);
	void Update();
	void draw();


	template<class T>
	std::shared_ptr<T> GetComponent()
	{
		for (auto comp : components)
		{
			std::shared_ptr<T> target = nullptr;
			if ((target = std::dynamic_pointer_cast<T>(comp)))
			{
				return(target);
			}
		}

		//Return NULL;
		return(nullptr);
	}
	
protected:
	std::vector<std::shared_ptr<Component>> components;
};

#endif // !GAMEOBJECT_H
