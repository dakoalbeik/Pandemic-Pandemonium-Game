#pragma once
#include"SDL.h"
#include "Box2D.h"
#include "PhysicsDevice.h"
#include "GameObject.h"
#include "SoundController.h"
#include "Initializers.h"
#include <iostream>

class ContactListener : public b2ContactListener
{
public:
	ContactListener(SoundController* soundController, ItemNumbers* itemNumbers) : soundController(soundController), itemNumbers(itemNumbers) {}

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);


private:


	SoundController* soundController{ nullptr };
	ItemNumbers* itemNumbers{ nullptr };
};