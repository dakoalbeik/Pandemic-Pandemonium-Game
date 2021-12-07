#include "View.h"
View::View(Vector2D position) : position {position}
{
	
}

View::~View()
{
}


Vector2D View::getPosition()
{
	return position;
}

void View::setPosition(Vector2D position)
{
	this->position = position;
}
