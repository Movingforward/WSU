#ifndef MENU_H
#define MENU_H

#include "StateManager.h"
#include <math.h>

class Menu
{
public:
	Menu();
	virtual int Run(sf:: RenderWindow &window);

private:
	bool playing;
	float speed;
	sf:: Event event;
	int selection;
	sf:: Clock clock;
	float elapsed;
	float x_movement;
	float y_movement;
	float x, y;
	int debauch;
	sf:: Texture texture;
	sf:: Sprite sprite;
};
#endif MENU_H