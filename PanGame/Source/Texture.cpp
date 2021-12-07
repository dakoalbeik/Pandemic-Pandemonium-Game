#include <iostream>
#include "Texture.h"

using namespace std;


Texture::~Texture()
{
	if (sprite != nullptr)
	{
		SDL_DestroyTexture(sprite);
		sprite = nullptr;
		width = 0;
		height = 0;
	}
}


Texture::Texture(GraphicsDevice* gDevice, const std::string& pathOrText, bool isAnimated, Vector2D clipInfo, const bool isSprite)
{
	if (isSprite)
	{
		//Load the image
		SDL_Surface* surface = IMG_Load(pathOrText.c_str());

		//If the image loaded
		if (surface == nullptr)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", pathOrText.c_str(), IMG_GetError());

		}
		else
		{
			//Color key image
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 255, 255));

			//Create an optimized image
			sprite = SDL_CreateTextureFromSurface(gDevice->getRenderer(), surface);
			initialzied = sprite != nullptr;
			//Free the loaded image; no longer needed
			SDL_FreeSurface(surface);
		}
	}

	//set the height and width from the texture
	SDL_QueryTexture(sprite, NULL, NULL, &width, &height);

	if (isAnimated) {
		width = clipInfo.x;
		height = clipInfo.y;
	}
}


void Texture::draw(SDL_Renderer* renderer, Vector2D position, float angle, SDL_Rect* clip, bool isFacingRight)
{
	if (sprite)
	{
		//set rendering space and render to screen
		SDL_Rect renderQuad = { (int)position.x, (int)position.y, width, height };

		//set clip rendering dimensions
		if (clip != nullptr) {
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
			width = clip->w;
			height = clip->h;
		}

		//Flip orientation based on object direction
		SDL_RendererFlip flip{ SDL_RendererFlip::SDL_FLIP_NONE };
		if (!isFacingRight) {
			flip = SDL_FLIP_HORIZONTAL;
		}


		//Render to screen
		SDL_RenderCopyEx(renderer, sprite, clip, &renderQuad, angle, NULL, flip);
	}


}

int Texture::getWidth() {
	return(width);
}

int Texture::getHeight() {
	return(height);
}