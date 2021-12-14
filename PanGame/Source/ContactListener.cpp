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
		soundController->playSound(Sound::BOUNCE, 0);
	}
	else if (objectTypeB == ObjectType::Player &&
		(objectTypeA == ObjectType::Platform || objectTypeA == ObjectType::Floor)) {
		objectB->GetComponent<MoveComponent>()->Jump();
		soundController->playSound(Sound::BOUNCE, 0);
	}

	//player collision with item (collecting)
	else if (objectTypeA == ObjectType::Player && objectTypeB == ObjectType::Item) {
		objectB->GetComponent<BodyComponent>()->dead = true;
	}
	else if (objectTypeB == ObjectType::Player && objectTypeA == ObjectType::Item) {
		objectA->GetComponent<BodyComponent>()->dead = true;
	}
	// decrease health on collision with karen
	else if (objectTypeA == ObjectType::Player && objectTypeB == ObjectType::Karen) {
		//health -= 50;
	}
	else if (objectTypeB == ObjectType::Player && objectTypeA == ObjectType::Karen) {
		//health -= 50;
	}
	// decrease health on collision with virus
	else if (objectTypeA == ObjectType::Player && objectTypeB == ObjectType::Virus) {
		health--;
	}
	else if (objectTypeB == ObjectType::Player && objectTypeA == ObjectType::Virus) {
		health--;
	}


	// ignore collsion for Karen with platforms
	else if (objectTypeA == ObjectType::Karen &&
		(objectTypeB == ObjectType::Platform ||
			objectTypeB == ObjectType::Virus ||
			objectTypeB == ObjectType::Item)) {
		contact->SetEnabled(false);
	}
	else if (objectTypeB == ObjectType::Karen &&
		(objectTypeA == ObjectType::Platform ||
			objectTypeB == ObjectType::Virus ||
			objectTypeA == ObjectType::Item)) {
		contact->SetEnabled(false);
	}


}

void ContactListener::BeginContact(b2Contact* contact) {
}

void ContactListener::EndContact(b2Contact* contact) {
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	//Grab the two Physics Bodies involved in the Contact
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	// find the objects
	GameObject* objectA = (GameObject*)bodyA->GetUserData().pointer;
	GameObject* objectB = (GameObject*)bodyB->GetUserData().pointer;

	// get the objects type
	ObjectType objectTypeA = objectA->GetComponent<BodyComponent>()->getObjectType();
	ObjectType objectTypeB = objectB->GetComponent<BodyComponent>()->getObjectType();

	//ignore collision with items or viruses after collection or damage taken
	if (objectTypeA == ObjectType::Player &&
		(objectTypeB == ObjectType::Item ||
			objectTypeB == ObjectType::Virus)) {
		contact->SetEnabled(false);
	}
	else if (objectTypeB == ObjectType::Player &&
		(objectTypeB == ObjectType::Item ||
			objectTypeB == ObjectType::Virus)) {
		contact->SetEnabled(false);
	}
}