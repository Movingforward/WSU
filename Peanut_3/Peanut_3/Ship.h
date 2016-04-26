#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"
#include "Collision.h"
#include <SFML/System/Vector2.hpp>
//For documentation on sf::Vector2, see
// http://sfml-dev.org/documentation/2.0/classsf_1_1Vector2.php
//Basically consists of x and y.

class Ship
{
public:
	Ship();
	~Ship();
	void update(sf::RenderWindow & window);
	sf::Vector2f move;

	int getdestroyed ();
	void setdestroyed (int state);
	void setPos (float xc, float yc);
	sf::Vector2f getPos ();
	sf::Sprite getImage ();
	void setImage (std::string filename);
	int health;

protected:
	sf::Sprite shipImage;
	sf::Texture texture;
	sf::Vector2f pos;

	
	int destroyed;
};

#endif