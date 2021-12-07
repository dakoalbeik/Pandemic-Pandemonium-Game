#include <cmath>
#include "ChaseComponent.h"
#include "PhysicsDevice.h"
#include "BodyComponent.h"




ChaseComponent::ChaseComponent(std::shared_ptr<GameObject> owner) : Component(owner) {
}

ChaseComponent::~ChaseComponent() {

}

void ChaseComponent::Update() {
	//if the chase component isn't disabled 
	if (!disabled) {
		Vector2D steering = pursuit();
		//sets the cap for the force added to the steering
		steering.Truncate(MAX_FORCE);


		Vector2D velocity = objectBody->getVelocity() + steering;
		//sets the cap for the speed
		velocity.Truncate(MAX_VELOCITY);

		objectBody->getPDevice()->setLinearVelocity(owner.get(), velocity);

		objectBody->getPDevice()->setTransform(owner.get(), { {objectBody->getPosition() + velocity}, 0 });
	}


}


Vector2D ChaseComponent::seek(Vector2D targetPosition) {
	//getting difference in the current position and position of target
	Vector2D positionDiff = targetPosition - objectBody->getPosition();
	//divides x and y by the hypotenuse
	positionDiff.Normalize();
	//desired velocity = distance between objects multiplied by max velocity (quickest way to get there)
	Vector2D desiredVelocity = positionDiff * MAX_VELOCITY;
	return desiredVelocity - objectBody->getVelocity();
}
Vector2D ChaseComponent::pursuit() {
	/*Vector2D distance = playerBody->getPosition() - objectBody->getPosition();*/
	/*int T = distance.Length() / MAX_VELOCITY;*/

	//constant time step
	float T = 1.05;
	//target position + (target velocity * time step)
	Vector2D futurePosition = targetBody->getPosition() + (targetBody->getVelocity() * T);
	//pass in future position and calculate quickest way to get there
	return seek(futurePosition);
}