#include "Ship.h"
/*
	Ship Constructors
	Description: Below is the constructor for the Ship class.			 
*/
Ship::Ship()
{
	texture.loadFromFile("ship.png");
	shipImage.setTexture(texture);
	shipImage.setPosition(350, 500);
	pos = shipImage.getPosition();
	move.x = 0;
	move.y = 0;
	health = 20;
	destroyed = 0;	
}
Ship::~Ship()
{}

/*
	Certain Setters & Getters
	Description: Below are necessary getters and setters for
				 certain class members.
*/
sf::Sprite Ship::getImage ()
{
	return shipImage;
}

void Ship::setImage (std::string filename)
{
	texture.loadFromFile (filename);
	shipImage.setTexture(texture);
}

void Ship::setPos (float xc, float yc)
{
	shipImage.setPosition (xc, yc);
	pos = shipImage.getPosition ();
}

sf::Vector2f Ship::getPos ()
{
	sf::Vector2f result;
	result = shipImage.getPosition ();
	return result;
}


int Ship::getdestroyed ()
{
	return destroyed;
}

void Ship::setdestroyed (int state)
{
	destroyed = state;
}

/*	void Ship::update(sf::RenderWindow & window)
	Description: This function updates the necessary values for the
				 player's ship so it acts as intended during gameplay.
*/	
/*Call every frame to update ship.*/
void Ship::update(sf::RenderWindow & window)
{
	float ship_size = 64;
	//Needs to check if hit
	//Needs to check if it will move outside screen.

	//Causes ship to wrap in x:
	if (move.x + pos.x > window.getSize().x - ship_size / 2)
	{
		pos.x = 0 - ship_size / 2;
		shipImage.setPosition(pos);
	}
	else if (move.x + pos.x < 0 - ship_size / 2)
	{
		pos.x = window.getSize().x - ship_size / 2;
		shipImage.setPosition(pos);
	}

	//Ship cannot leave screen along y:
	if (move.y + pos.y > window.getSize().y  - ship_size || move.y + pos.y < 0)
	{
		move.y = 0;
	}

	if (health <= 0)
	{
		destroyed = 1;
		texture.loadFromFile("explosion.jpg");
		shipImage.setTexture (texture);
		//shipImage.setPosition (-500,-500);
	}





	shipImage.move(move);
	pos += move;
	

	window.draw(shipImage);
}


