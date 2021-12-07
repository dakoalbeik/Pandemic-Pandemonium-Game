#pragma once
#ifndef VIEW_H
#define VIEW_H

#include "Vector2D.h"
#include "InputDevice.h"

#include "Definitions.h"
class View {
public:

	//Constructors
	View(Vector2D position);
	~View();

	//Setters & Getters
	Vector2D getPosition();
	void setPosition(Vector2D position);

	Vector2D position;

};


#endif // !VIEW_H


