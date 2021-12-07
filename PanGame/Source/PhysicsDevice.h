#pragma once

#include "Vector2D.h"
#include "box2d.h"
#include "Initializers.h"
#include "ContactListener.h"

#include <memory>

class GameObject;

class PhysicsDevice
{
public:
	PhysicsDevice(Vector2D gravity);
	bool initialize();
	bool update(float dt);

	bool createFixture(GameObject* object, PhysicsPresets physics, GamePosition position);
	bool accelerate(GameObject* object, Vector2D forceVec);
	void applyForce(GameObject* object, bool forward);
	void applyAngularForce(GameObject* object, bool right);

	//getters
	Vector2D getPosition(GameObject* object);
	float getAngle(GameObject* object);
	Vector2D getVelocity(GameObject* object);
	float getAngularVelocity(GameObject* object);

	//setters
	bool setLinearVelocity(GameObject* object, Vector2D linearVelocity);
	bool setLinearImpulse(GameObject* object, Vector2D forceVec, Vector2D forceCenter);
	bool setAngularVelocity(GameObject* object, float angularVelocity);
	void setEnabled(GameObject* object, bool isEnabled);
	bool setTransform(GameObject* object, GamePosition gPosition);
	void setFixedRotation(GameObject* object, bool rotationLocked);

	//Misc
	b2Body* findBody(GameObject* object);
	bool removeObject(GameObject* object);
	//conversions between phsysics world and SDL2
	Vector2D PV2GV(b2Vec2 physicsVec);//Physics Vector to Game Vector
	b2Vec2 GV2PV(Vector2D gameVec); //Game Vector to Physics Vector


	//public just for creating borders!
	b2World* world;
private:

	const b2Vec2 gravity;
	std::unique_ptr<ContactListener> contactListener{ nullptr };
	Vector2D alignCenters(GameObject* object); //takes in Physics coordinates and returns SDL coordinates
};