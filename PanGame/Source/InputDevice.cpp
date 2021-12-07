#include "InputDevice.h"
#include <iostream>

//Methods

InputDevice::InputDevice()
{
	event = std::make_unique<SDL_Event>();
	if (!event) {
		printf("SDL Event could not initialize!");
		exit(1);
	}
	keyStates[gameEvent::UP] = false;
	keyStates[gameEvent::DOWN] = false;
	keyStates[gameEvent::LEFT] = false;
	keyStates[gameEvent::RIGHT] = false;
	keyStates[gameEvent::SPACE] = false;
	keyStates[gameEvent::NA] = true;
	keyStates[gameEvent::SHIFT] = false;
	keyStates[gameEvent::QUIT] = false;

	Update();
}

void InputDevice::Update()
{




	gameEvent gEvent;
	if (SDL_PollEvent(event.get()))
	{
		switch (event->type)
		{
		case SDL_KEYDOWN:
			gEvent = keyTranslate();
			keyStates.find(gEvent)->second = true;
			break;
		case SDL_KEYUP:
			gEvent = keyTranslate();
			keyStates.find(gEvent)->second = false;
			break;
		case SDL_QUIT:
			keyStates.find(gameEvent::QUIT)->second = true;
			break;
		default:
			break;
		}


	}

	//change map with key NA to true when no keys are pressed
	keyStates[gameEvent::NA] = true;

	for (auto itr{ keyStates.begin() }; itr != keyStates.end(); itr++)
	{
		if (itr->second && itr->first != gameEvent::NA) {
			keyStates[gameEvent::NA] = false;
		}
	}



}


bool InputDevice::GetEvent(gameEvent event)
{
	return (keyStates.find(event)->second);
}


InputDevice::gameEvent InputDevice::keyTranslate()
{
	switch (event->key.keysym.sym)
	{
	case SDLK_LEFT:
		return gameEvent::LEFT;
		break;
	case SDLK_RIGHT:
		return gameEvent::RIGHT;
		break;
	case SDLK_UP:
		return gameEvent::UP;
		break;
	case SDLK_DOWN:
		return gameEvent::DOWN;
		break;
	case SDLK_SPACE:
		return gameEvent::SPACE;
		break;
	case SDLK_RSHIFT:
	case SDLK_LSHIFT:
		return gameEvent::SHIFT;
		break;
	}
	return gameEvent::NA;
}


