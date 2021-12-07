#pragma once
#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include<memory>
#include<map>
#include "SDL.h"
#include "Vector2D.h"


class InputDevice {
public:

	//Members -----------------------------------------------------------------------------------
	enum class gameEvent
	{
		NA,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SPACE,
		SHIFT,
		QUIT,
		NUM_EVENTS
	};


	//Members -----------------------------------------------------------------------------------


	//Constructor--------------------------------------------------------------------------------

	InputDevice();

	//Constructor-------------------------------------------------------------------------------


	//Methods------------------------------------------------------------------------------------

	void Update();
	bool GetEvent(gameEvent event);

	//Methods------------------------------------------------------------------------------------


private:
	std::unique_ptr<SDL_Event> event;
	std::map<gameEvent, bool> keyStates;
	gameEvent keyTranslate();
};


#endif // !INPUTDEVICE_H