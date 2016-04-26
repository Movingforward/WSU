#include "Bullet.h"
#include <cmath>
/*
	Bullet Constructors
	Description: Below are the 3 constructors for the Bullet class.			 
*/
Bullet::Bullet (sf::Vector2f position, Ship ship)
{
	sf::Vector2f tempV;
	sf::Vector2f shift;
	float templength = 0;

	shift.x = 32;
	shift.y = 32;

	texture.loadFromFile("bullet1.png");

	shipImage.setTexture(texture);
	shipImage.setPosition(position + shift);
	pos = position + shift;
	tempV = pos - ship.getPos();
	templength = sqrt(tempV.x * tempV.x + tempV.y * tempV.y);

	tempV.x /= templength;
	tempV.y /= templength;
	move = tempV * (float) -3;
	//move.x = 0;
	//move.y = .3;
	destruct_rating = 1;	
}

Bullet::Bullet (sf::Vector2f position)
{
	
	sf::Vector2f shift;

	float templength = 0;
	shift.x = 32;
	shift.y = 32;
	texture.loadFromFile("bullet1.png");

	shipImage.setTexture(texture);
	shipImage.setPosition(position + shift);
	pos = position;

	move.x = 0;
	
	move.y = (float) -10;
	destruct_rating = 1;
	
}
//The following constructor was used before all Bullets were placed into a Vector.
Bullet::Bullet ()
{
	texture.loadFromFile("bullet1.png");

	shipImage.setTexture(texture);
	shipImage.setPosition(-100, -100);
	pos = shipImage.getPosition();
	move.x = 0;
	move.y = 0;
	
	
	destruct_rating = 1;
}

Bullet::~Bullet ()
{}

/*
	void Bullet::update(sf::RenderWindow & window)
	Description: This function updates the necessary values for the
				 bullet so it acts as intended during gameplay.			 
*/
void Bullet::update(sf::RenderWindow & window)
{
	//	float ship_size = 64;
	////Needs to check if hit
	////Needs to check if it will move outside screen.

	////Causes ship to wrap in x:
	//if (move.x + pos.x > window.getSize().x - ship_size / 2)
	//{
	//	pos.x = 0 - ship_size / 2;
	//	shipImage.setPosition(pos);
	//}
	//else if (move.x + pos.x < 0 - ship_size / 2)
	//{
	//	pos.x = window.getSize().x - ship_size / 2;
	//	shipImage.setPosition(pos);
	//}

	////Ship cannot leave screen along y:
	//if (move.y + pos.y > window.getPosition().y - ship_size || move.y + pos.y < 0)
	//{
	//	move.y = 0;
	//}
	
	

	shipImage.move(move);
	pos += move;
	window.draw(shipImage);
	
}
/*
	Certain Setters & Getters
	Description: Below are necessary getters and setters for
				 certain class members.
*/
void Bullet::set_image (std::string filename)
{
	texture.loadFromFile(filename);
	shipImage.setTexture(texture);
}

void Bullet::setPos (float x, float y)
{
	shipImage.setPosition (x, y);
	pos = shipImage.getPosition ();
}

sf::Sprite Bullet::getImage ()
{
	return shipImage;
}
