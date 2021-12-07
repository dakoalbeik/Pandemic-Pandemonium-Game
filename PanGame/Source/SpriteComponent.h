#pragma once
#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "ComponentList.h"
#include "GameObject.h"
#include "SDL.h"
#include "tinyxml2.h"
#include "Initializers.h"
#include "Texture.h"


#include "Definitions.h"
class SpriteComponent : public Component {
public:
	SpriteComponent(std::shared_ptr<GameObject> owner, GraphicsDevice* gDevice, SpritePresets& presets);
	~SpriteComponent();

	void Update();
	void draw();

	Vector2D getViewAdjustedPosition();
	std::shared_ptr<Texture> getTexture();

	//instance of the sprite body needed to update object position
	std::shared_ptr<BodyComponent> spriteBody{ nullptr };

	//instance needed to determine orientation
	std::shared_ptr<UserInputComponent> inputComponent{ nullptr };
	std::shared_ptr<ChaseComponent> chaseComponent{ nullptr };

	std::shared_ptr<GameObject> owner{ nullptr };

	Uint32 spriteID{ 0 };  //number of current sprite displayed
	bool isAnimated{ false };  //flag if it has a sprite sheet
private:
	std::shared_ptr<Texture> texture{ nullptr };
	void flipThruSprites();  //flips through sprites by changing spriteID
	std::vector<SDL_Rect> clipVector;  //clipVector size determined by number of sprites in xml
	GraphicsDevice* gDevice;

	bool isFacingRight{ true };  //used to flip texture based on orientation

	//info used for animation
	int initSpriteID{ 0 };
	int animLag{ 0 };
	int numOfCols{ 0 };
	Uint32 stepLagCount{ 0 };
	Uint32 stepCount{ 0 };

};
#endif // !SPRITECOMPONENT_H