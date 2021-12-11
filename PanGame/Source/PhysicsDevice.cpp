#include "PhysicsDevice.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "Texture.h"

#include <iostream>

PhysicsDevice::PhysicsDevice(Vector2D gravity) :gravity(RW2PW(gravity.x), RW2PW(+gravity.y)) {}

//Creates a new world on initialization
bool PhysicsDevice::initialize(SoundController* soundController) {

	world = new b2World(gravity);
	if (world == nullptr) return false;

	contactListener = std::make_unique<ContactListener>(soundController);
	world->SetContactListener(contactListener.get());

	return true;
}

//Steps the physics world
bool PhysicsDevice::update(float dt) {
	world->Step(dt, 8, 3);
	return true;
}

//Creates a body and fixture for a passed in object, based on passed in values
bool PhysicsDevice::createFixture(GameObject* object, PhysicsPresets physics, GamePosition positionElements) {
	//need the sprite component first!
	if (object->GetComponent<SpriteComponent>() == nullptr) return false;
	//new body definition
	b2BodyDef* bd = new b2BodyDef;


	//Set userData field with passed in object pointer.

	//void* so we can store anything! But must cast back to what it is to use!
	bd->userData.pointer = reinterpret_cast<uintptr_t>(object);


	//set body type
	switch (physics.physicsBodyType)
	{
	case GAME_STATIC:
		bd->type = b2_staticBody;
		break;
	case GAME_KINEMATIC:
		bd->type = b2_kinematicBody;
		break;
	case GAME_DYNAMIC:
		bd->type = b2_dynamicBody;
		break;
	}

	//********Adjust postion because SDL is top left, while box2d is center*************
	std::shared_ptr<Texture> texture = object->GetComponent<SpriteComponent>()->getTexture();

	//subtract off half the width.
	auto [x, y] = positionElements.position;
	// set starting position & angle
	bd->position.Set(RW2PW((float)(x + (texture->getWidth() / 2))), RW2PW((float)(y + (texture->getHeight() / 2))));
	bd->angle = RW2PWAngle(positionElements.angle);

	//add the body to the world
	b2Body* body = world->CreateBody(bd);

	//Set damping values on the body
	body->SetAngularDamping(physics.angularDamping);
	body->SetLinearDamping(physics.linearDamping);

	//Fixture definition
	b2FixtureDef shapefd;
	//made need one or the other, depending on what was passed.
	b2PolygonShape pShape;
	b2CircleShape cShape;
	//Set fixture's shape
	switch (physics.objectShape)
	{
	case GAME_RECTANGLE:
		//rectangle's dimensions
		pShape.SetAsBox(RW2PW(texture->getWidth() / 2), RW2PW(texture->getHeight() / 2));
		shapefd.shape = &pShape;
		break;
	case GAME_CIRCLE:
		//circle radius based on object's width.
		float width = object->GetComponent<SpriteComponent>()->getTexture()->getWidth() / 2.0f;
		float height = object->GetComponent<SpriteComponent>()->getTexture()->getHeight() / 2.0f;

		if (width > height)	cShape.m_radius = (RW2PW(width));
		else cShape.m_radius = (RW2PW(height));
		shapefd.shape = &cShape;
		break;
	}

	//set fixture values based on passed in values.
	shapefd.density = physics.density;
	shapefd.friction = physics.friction;
	shapefd.restitution = physics.restitution;

	//create the fixture on the body.
	body->CreateFixture(&shapefd);
	body->SetEnabled(physics.physicsOn);



	return true;

}

void PhysicsDevice::applyForce(GameObject* object, bool forward)
{
	int forceMultiplier = 10;
	b2Vec2 applyForce;
	if (forward)
	{
		applyForce.x = (cosf(getAngle(object) * PI / 180) - (PI / 2)) * forceMultiplier;
		applyForce.y = (sinf(getAngle(object) * PI / 180) - (PI / 2)) * forceMultiplier;
	}
	else
	{
		applyForce.x = (cosf(getAngle(object) * PI / 180) + (PI / 2)) * forceMultiplier;
		applyForce.y = (sinf(getAngle(object) * PI / 180) + (PI / 2)) * forceMultiplier;
	}
	findBody(object)->ApplyLinearImpulse(applyForce, findBody(object)->GetWorldCenter(), true);
}

void PhysicsDevice::applyAngularForce(GameObject* object, bool right)
{
	float forceMultiplier = 1;
	if (!right) forceMultiplier *= -1;
	forceMultiplier = findBody(object)->GetAngularVelocity() + (forceMultiplier * 2.0f * (1 / 50));
	findBody(object)->SetAngularVelocity(forceMultiplier);
}

Vector2D PhysicsDevice::alignCenters(GameObject* object)
{
	//needed for position in physics world
	b2Vec2 physPosition = findBody(object)->GetPosition();
	//needed for width/height of object;
	std::shared_ptr<Texture> texture = object->GetComponent<SpriteComponent>()->getTexture();

	//adjust x & y positions by half the sprite's width and heigh respectively to get the top left corner!
	Vector2D position{ PW2RW(physPosition.x) - (texture->getWidth() / 2), PW2RW(physPosition.y) - (texture->getHeight() / 2) };
	return (position);
}

bool PhysicsDevice::accelerate(GameObject* object, Vector2D forceVec)
{
	setLinearImpulse(object, forceVec, PV2GV(findBody(object)->GetPosition()));
	return false;
}

void PhysicsDevice::setFixedRotation(GameObject* object, bool rotationLocked) {
	b2Body* body = findBody(object);
	body->SetFixedRotation(rotationLocked);
}

//getters
Vector2D PhysicsDevice::getPosition(GameObject* object)
{
	return (alignCenters(object));
}

float PhysicsDevice::getAngle(GameObject* object)
{

	return (PW2RWAngle(findBody(object)->GetAngle()));
}

Vector2D PhysicsDevice::getVelocity(GameObject* object) {

	b2Body* body = findBody(object);
	return (PV2GV(body->GetLinearVelocity()));
}

float PhysicsDevice::getAngularVelocity(GameObject* object) {

	b2Body* body = findBody(object);
	return (PW2RWAngle(body->GetAngularVelocity()));
}


//setters


bool PhysicsDevice::setTransform(GameObject* object, GamePosition gPosition)
{
	//finds which body this object is attached to.
	//based on value set in UserData field of body
	b2Body* body = findBody(object);
	std::shared_ptr<Texture> texture = object->GetComponent<SpriteComponent>()->getTexture();

	//subtract off half the width.
	gPosition.position.x += (texture->getWidth() / 2);
	//subtract off half the height
	gPosition.position.y += (texture->getHeight() / 2);
	//Applies' Box2D transform.

	body->SetTransform
	(
		GV2PV(gPosition.position),
		RW2PWAngle(gPosition.angle)
	);

	return true;

}

bool PhysicsDevice::setLinearVelocity(GameObject* object, Vector2D linearVelocity) {

	b2Body* body = findBody(object);
	body->SetLinearVelocity({ RW2PW(linearVelocity.x), RW2PW(linearVelocity.y) });
	return true;
}

bool PhysicsDevice::setLinearImpulse(GameObject* object, Vector2D forceVec, Vector2D forceCenter) {

	b2Body* body = findBody(object);
	body->ApplyLinearImpulse
	(
		GV2PV(forceVec),
		body->GetWorldPoint(GV2PV(forceCenter)),
		true
	);
	return true;
}

bool PhysicsDevice::setAngularVelocity(GameObject* object, float angularVelocity) {

	b2Body* body = findBody(object);
	body->SetAngularVelocity(RW2PWAngle(angularVelocity));
	return true;

}

void PhysicsDevice::setEnabled(GameObject* object, bool isEnabled) {
	b2Body* body = findBody(object);
	body->SetEnabled(isEnabled);
}


//Misc

b2Body* PhysicsDevice::findBody(GameObject* object)
{
	for (b2Body* body = world->GetBodyList(); body; body = body->GetNext())
	{
		//when we have a match, return it.
		if (reinterpret_cast<uintptr_t>(object) == body->GetUserData().pointer)
		{
			return body;
		}
	}
	//if there was no match, return nullptr
	return nullptr;
}

bool PhysicsDevice::removeObject(GameObject* object)
{
	//Find the body based on the passed object
	b2Body* body = findBody(object);

	//check to see if we have a body.
	if (body == nullptr)
	{
		return false;
	}
	else
	{
		//delete the body from the world
		world->DestroyBody(body);
		return true;
	}
}

Vector2D PhysicsDevice::PV2GV(b2Vec2 physicsVec)
{
	return { PW2RW(physicsVec.x), PW2RW(physicsVec.y) };
}

b2Vec2 PhysicsDevice::GV2PV(Vector2D gameVec) {

	return b2Vec2(RW2PW((float)gameVec.x), RW2PW((float)gameVec.y));
}