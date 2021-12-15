#include <string>
#include "GraphicsDevice.h"

#include <iostream>


GraphicsDevice::GraphicsDevice(Uint32 width, Uint32 height, bool fullScreen = true) : SCREEN_WIDTH(width), SCREEN_HEIGHT(height)
{
	//initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	//initialize SDL_image subsystems
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_Error: %s\n", IMG_GetError());
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		exit(1);
	}

	//Initialize Font----------File path-----------font size
	scoreFont = TTF_OpenFont("./Assets/Fonts/impact.ttf", 19);
	textFont = TTF_OpenFont("./Assets/Fonts/impact.ttf", 15);
	deathFont = TTF_OpenFont("./Assets/Fonts/impact.ttf", 100);

	if (scoreFont == nullptr || textFont == nullptr || deathFont == nullptr) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	if (!fullScreen)
	{
		//Construct and check window construction
		screen = SDL_CreateWindow("Pandemic Pandemonium",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		screen = SDL_CreateWindow("Pandemic Pandemonium",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	}
	if (screen == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//Construct the renderer
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	//set the background color (default blue)
	SDL_SetRenderDrawColor(renderer, 145, 206, 235, 235);
	//set the background color (default white)
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


	////Load background image
	//backImg = IMG_Load("Hospital.png");
	////Create background texture
	//backTexture = SDL_CreateTextureFromSurface(renderer, backImg);
	////Render to window
	//SDL_RenderCopy(renderer, backTexture, NULL, NULL);




	//create view
	Vector2D position{ 0, 0 };
	view = std::make_unique<View>(position);
}

GraphicsDevice::~GraphicsDevice()
{
	//Free the window
	SDL_DestroyWindow(screen);
	screen = nullptr;

	//Free renderer
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	//clean up text
	TTF_CloseFont(scoreFont);
	TTF_CloseFont(textFont);
	TTF_CloseFont(deathFont);

	////Clean up background
	//SDL_DestroyTexture(backTexture);
	//SDL_FreeSurface(backImg);

	//Quit SDL Subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void GraphicsDevice::Begin()
{
	SDL_RenderClear(renderer);
}


void GraphicsDevice::Present()
{
	SDL_RenderPresent(renderer);
}

SDL_Renderer* GraphicsDevice::getRenderer()
{
	return(renderer);
}

View* GraphicsDevice::getView()
{
	return view.get();
}

void GraphicsDevice::setView(View* view)
{
	this->view = std::unique_ptr<View>(view);
}

void GraphicsDevice::update(ItemNumbers* itemNumbers) {

	//-------------------HIGHSCORE RENDERING------------------------------------------

	std::string hScore = "Highscore " + std::to_string(highScore);

	//Render Command to Text     
	SDL_Color textColor = { 37, 44, 54 };

	SDL_Surface* hScoreSurface{ TTF_RenderText_Solid(textFont, hScore.c_str(), textColor) };

	SDL_Texture* hScoreTexture = SDL_CreateTextureFromSurface(renderer, hScoreSurface);

	//Set rendering space and render to screen
	int width = 0, height = 0;
	SDL_QueryTexture(hScoreTexture, nullptr, nullptr, &width, &height);
	SDL_Rect hScoreQuad = { SCREEN_WIDTH - width - 15, 55, width, height };

	//Render to screen
	SDL_RenderCopy(renderer, hScoreTexture, nullptr, &hScoreQuad);

	SDL_FreeSurface(hScoreSurface);
	SDL_DestroyTexture(hScoreTexture);


	//-----------------CURRENT SCORE RENDERING----------------------------------------

	std::string cScore = "Current Score " + std::to_string(score);

	SDL_Surface* cScoreSurface{ TTF_RenderText_Solid(textFont, cScore.c_str(), textColor) };

	//Render Command to Text
	SDL_Texture* cScoreTexture = SDL_CreateTextureFromSurface(renderer, cScoreSurface);

	//Set rendering space and render to screen
	SDL_QueryTexture(cScoreTexture, nullptr, nullptr, &width, &height);
	SDL_Rect cScoreQuad = { SCREEN_WIDTH - width - 15, 80, width, height };

	SDL_RenderCopy(renderer, cScoreTexture, nullptr, &cScoreQuad);

	SDL_FreeSurface(cScoreSurface);
	SDL_DestroyTexture(cScoreTexture);


	//----------------------------HEALTH BAR----------------------------------------

	// (health / maxHealth) * maxColorValue desired for color transition
	int healthColorVar{ (int)(((float)health / 100) * 235) };

	if (health <= 0) {
		health = 0;
		healthColorVar = 0;
	}

	std::string healthBar = "Health " + std::to_string(health) + "%";
	SDL_Color healthColor{ 255, healthColorVar, healthColorVar };

	SDL_Surface* healthBarSurface{ TTF_RenderText_Solid(scoreFont, healthBar.c_str(), healthColor) };

	//Render Command to Text
	SDL_Texture* healthBarTexture = SDL_CreateTextureFromSurface(renderer, healthBarSurface);

	//Set rendering space and render to screen
	SDL_QueryTexture(healthBarTexture, nullptr, nullptr, &width, &height);
	SDL_Rect healthBarQuad = { 5, 55, width, height };

	SDL_RenderCopy(renderer, healthBarTexture, nullptr, &healthBarQuad);

	SDL_FreeSurface(healthBarSurface);
	SDL_DestroyTexture(healthBarTexture);

	//--------------------ITEM NUMBERS----------------------------------------------

	itemNums(itemNumbers);

	//--------------------DEATH RENDERING-------------------------------------------

	std::string death{ NULL };
	if (isPlayerDead) {
		death = "YOU LOSE!";
	}

	//Render Command to Death Text
	SDL_Color deathColor = { 0, 0, 0 };
	SDL_Surface* deathSheetSurface{ TTF_RenderText_Solid(deathFont, death.c_str(), deathColor) };
	SDL_Texture* deathSheetTexture = SDL_CreateTextureFromSurface(renderer, deathSheetSurface);

	//Set rendering space and render to screen
	SDL_QueryTexture(deathSheetTexture, nullptr, nullptr, &width, &height);
	SDL_Rect deathQuad = { SCREEN_WIDTH - 475, 425, width, height };


	SDL_RenderCopy(renderer, deathSheetTexture, nullptr, &deathQuad);

	SDL_FreeSurface(deathSheetSurface);
	SDL_DestroyTexture(deathSheetTexture);
}

void GraphicsDevice::itemNums(ItemNumbers* itemNumbers) {


	SDL_Color itemsColor{ 37, 44, 54 };
	int width = 0, height = 0;                // x = 79, y = 60

	//--------------------------------BUNDLE----------------------------------------------

	std::string bundle = std::to_string(itemNumbers->Bundle);
	SDL_Surface* bundleSurface{ TTF_RenderText_Solid(scoreFont, bundle.c_str(), itemsColor) };
	SDL_Texture* bundleTexture = SDL_CreateTextureFromSurface(renderer, bundleSurface);
	SDL_QueryTexture(bundleTexture, nullptr, nullptr, &width, &height);
	SDL_Rect bundleQuad = { 65, 13, width, height };
	SDL_RenderCopy(renderer, bundleTexture, nullptr, &bundleQuad);
	SDL_FreeSurface(bundleSurface);
	SDL_DestroyTexture(bundleTexture);


	//--------------------------------mRNA----------------------------------------------

	std::string mRNA = std::to_string(itemNumbers->mRNA);
	SDL_Surface* mRNASurface{ TTF_RenderText_Solid(scoreFont, mRNA.c_str(), itemsColor) };
	SDL_Texture* mRNATexture = SDL_CreateTextureFromSurface(renderer, mRNASurface);
	SDL_QueryTexture(mRNATexture, nullptr, nullptr, &width, &height);
	SDL_Rect mRNAQuad = { 165, 13, width, height };
	SDL_RenderCopy(renderer, mRNATexture, nullptr, &mRNAQuad);
	SDL_FreeSurface(mRNASurface);
	SDL_DestroyTexture(mRNATexture);

	//--------------------------------PAPERS----------------------------------------------

	std::string papers = std::to_string(itemNumbers->Papers);
	SDL_Surface* papersSurface{ TTF_RenderText_Solid(scoreFont, papers.c_str(), itemsColor) };
	SDL_Texture* papersTexture = SDL_CreateTextureFromSurface(renderer, papersSurface);
	SDL_QueryTexture(papersTexture, nullptr, nullptr, &width, &height);
	SDL_Rect papersQuad = { 265, 13, width, height };
	SDL_RenderCopy(renderer, papersTexture, nullptr, &papersQuad);
	SDL_FreeSurface(papersSurface);
	SDL_DestroyTexture(papersTexture);

	//--------------------------------SYRINGE----------------------------------------------

	std::string syringe = std::to_string(itemNumbers->Syringe);
	SDL_Surface* syringeSurface{ TTF_RenderText_Solid(scoreFont, syringe.c_str(), itemsColor) };
	SDL_Texture* syringeTexture = SDL_CreateTextureFromSurface(renderer, syringeSurface);
	SDL_QueryTexture(syringeTexture, nullptr, nullptr, &width, &height);
	SDL_Rect syringeQuad = { 365, 13, width, height };
	SDL_RenderCopy(renderer, syringeTexture, nullptr, &syringeQuad);
	SDL_FreeSurface(syringeSurface);
	SDL_DestroyTexture(syringeTexture);

	//--------------------------------TESTTUBE----------------------------------------------

	std::string testtube = std::to_string(itemNumbers->TestTube);
	SDL_Surface* testtubeSurface{ TTF_RenderText_Solid(scoreFont, testtube.c_str(), itemsColor) };
	SDL_Texture* testtubeTexture = SDL_CreateTextureFromSurface(renderer, testtubeSurface);
	SDL_QueryTexture(testtubeTexture, nullptr, nullptr, &width, &height);
	SDL_Rect testtubeQuad = { 465, 13, width, height };
	SDL_RenderCopy(renderer, testtubeTexture, nullptr, &testtubeQuad);
	SDL_FreeSurface(testtubeSurface);
	SDL_DestroyTexture(testtubeTexture);

	//--------------------------------Mask----------------------------------------------

	std::string mask = std::to_string(itemNumbers->Mask);
	SDL_Surface* maskSurface{ TTF_RenderText_Solid(scoreFont, mask.c_str(), itemsColor) };
	SDL_Texture* maskTexture = SDL_CreateTextureFromSurface(renderer, maskSurface);
	SDL_QueryTexture(maskTexture, nullptr, nullptr, &width, &height);
	SDL_Rect maskQuad = { 565, 13, width, height };
	SDL_RenderCopy(renderer, maskTexture, nullptr, &maskQuad);
	SDL_FreeSurface(maskSurface);
	SDL_DestroyTexture(maskTexture);
}