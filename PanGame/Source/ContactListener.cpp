#include "ContactListener.h"

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{


	//Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	// find the objects
	GameObject* objectA = (GameObject*)bodyA->GetUserData().pointer;
	GameObject* objectB = (GameObject*)bodyB->GetUserData().pointer;

	// get the objects type
	ObjectType objectTypeA = objectA->GetComponent<BodyComponent>()->getObjectType();
	ObjectType objectTypeB = objectB->GetComponent<BodyComponent>()->getObjectType();


	// make player jump upon contact with platforms
	if (objectTypeA == ObjectType::Player &&
		(objectTypeB == ObjectType::Platform || objectTypeB == ObjectType::Floor)) {
		objectA->GetComponent<MoveComponent>()->Jump();
	}
	else if (objectTypeB == ObjectType::Player &&
		(objectTypeA == ObjectType::Platform || objectTypeA == ObjectType::Floor)) {
		objectB->GetComponent<MoveComponent>()->Jump();
	}


	// ignore collsion for Karen with platforms
	else if (objectTypeA == ObjectType::Karen &&
		(objectTypeB == ObjectType::Platform || objectTypeB == ObjectType::Item)) {
		contact->SetEnabled(false);
	}
	else if (objectTypeB == ObjectType::Karen &&
		(objectTypeA == ObjectType::Platform || objectTypeA == ObjectType::Item)) {
		contact->SetEnabled(false);
	}
}
