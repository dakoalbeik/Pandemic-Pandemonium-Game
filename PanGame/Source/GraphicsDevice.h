#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "View.h"
#include "Definitions.h"
#include "Initializers.h"

class GraphicsDevice
{

public:
	GraphicsDevice(Uint32 width, Uint32 height, bool fullScreen);
	GraphicsDevice() = delete;
	~GraphicsDevice();

	//Rendering functions
	void Begin();
	void Present();

	SDL_Renderer* getRenderer();
	View* getView();

	void setView(View* view);
	void update(ItemNumbers* itemNumbers);



private:
	//Window(s) to display graphics
	SDL_Window* screen{ nullptr };
	SDL_Renderer* renderer{ nullptr };
	//The pointers to be used for the text
	TTF_Font* textFont{ nullptr };
	TTF_Font* deathFont{ nullptr };

	void itemNums(ItemNumbers* itemNumbers);
	//SDL_Surface* backImg{ nullptr };
	//SDL_Texture* backTexture{ nullptr };

	std::unique_ptr<View> view{ nullptr };
	//Parameters
	const Uint32 SCREEN_WIDTH{ 0 };
	const Uint32 SCREEN_HEIGHT{ 0 };

};
