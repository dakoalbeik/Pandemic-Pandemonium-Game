#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "GraphicsDevice.h"
#include "Vector2D.h"

#include "Definitions.h"
class Texture {

public:

	Texture() = delete;
	Texture(GraphicsDevice* gDevice, const std::string& pathOrText, bool isAnimated, Vector2D clipInfo, const bool isSprite = true);
	~Texture();

	//added parameter to determine orientation
	void draw(SDL_Renderer* renderer, Vector2D position, float angle, SDL_Rect* clip = NULL, bool isFacingRight = true);

	int getWidth();
	int getHeight();

	bool initialzied{ false };
private:
	//The actual hardware texture. created and destroyed by SDL
	SDL_Texture* sprite{ nullptr };
	int width{ 0 };
	int height{ 0 };

};
#endif