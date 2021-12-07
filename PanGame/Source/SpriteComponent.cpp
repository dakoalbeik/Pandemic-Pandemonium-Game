#include "SpriteComponent.h"
#include "BodyComponent.h"
#include "Initializers.h"

#include <iostream>

SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner, GraphicsDevice* gDevice, SpritePresets& presets) :
	Component(owner), gDevice(gDevice) {

	isAnimated = presets.isAnimated;

	this->texture = presets.spriteTexture;

	inputComponent = owner->GetComponent<UserInputComponent>();
	chaseComponent = owner->GetComponent<ChaseComponent>();

	if (isAnimated) {
		for (int i{ 0 }; i < presets.clip->numSprites; i++) {
			int row = i / presets.clip->col_num;
			int col = i % presets.clip->col_num;

			clipVector.push_back(SDL_Rect());
			clipVector.back().x = col * presets.clip->width;
			clipVector.back().y = row * presets.clip->height;
			clipVector.back().w = presets.clip->width;
			clipVector.back().h = presets.clip->height;
		}
		animLag = presets.clip->animLag;
		initSpriteID = presets.clip->spriteID;
		spriteID = initSpriteID;
		numOfCols = presets.clip->col_num;
	}
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Update()
{

	if (inputComponent) {
		if (inputComponent->getInputDevice()->GetEvent(InputDevice::gameEvent::LEFT)) {
			isFacingRight = false;
			flipThruSprites();
		}
		else if (inputComponent->getInputDevice()->GetEvent(InputDevice::gameEvent::RIGHT)) {
			isFacingRight = true;
			flipThruSprites();
		}
		else if (inputComponent->getInputDevice()->GetEvent(InputDevice::gameEvent::NA)) {
			spriteID = 48;
		}
	}
	else if (chaseComponent) {
		flipThruSprites();
	}

}

void SpriteComponent::draw()
{
	Vector2D updatedPosition;
	updatedPosition = getViewAdjustedPosition();
	float angle = spriteBody->getAngle();


	if (isAnimated) {
		this->texture->draw(gDevice->getRenderer(), updatedPosition, angle, &clipVector.at(spriteID), isFacingRight);
	}
	else {
		this->texture->draw(gDevice->getRenderer(), updatedPosition, angle, NULL);
	}
}

Vector2D SpriteComponent::getViewAdjustedPosition()
{
	return (spriteBody->getPosition() + gDevice->getView()->getPosition());
}

std::shared_ptr<Texture> SpriteComponent::getTexture()
{
	return texture;
}

void SpriteComponent::flipThruSprites() {

	stepLagCount++;
	stepLagCount %= animLag;
	if (stepLagCount == 0) {
		spriteID++;
		if (spriteID >= initSpriteID + numOfCols) {
			spriteID = initSpriteID;
		}
	}
}
