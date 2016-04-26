#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

#include "Ship.h"
#include <SFML/System/Vector2.hpp>

class Bullet 
{
	public:
		Bullet ();
		Bullet (sf::Vector2f pos);
		Bullet (sf::Vector2f pos, Ship ship);
		~Bullet ();
		void update(sf::RenderWindow & window);
		sf::Vector2f pos;
		sf::Vector2f move;
		void set_image (std::string filename);
		void setPos (float x, float y);
		sf::Sprite getImage ();
		int destruct_rating;
	protected:
		
		sf::Sprite shipImage;
		sf::Texture texture;
};






#endif