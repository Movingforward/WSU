#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include "GUI.h"
#include <SFML/Graphics.hpp>

class StateManager
{
public:
	virtual int run(sf:: RenderWindow &window) = 0;
};


#endif STATEMANAGER_H
