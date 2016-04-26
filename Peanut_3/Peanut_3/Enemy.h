#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Ship.h"
#include "Bullet.h"
#include "Collision.h"
#include <SFML/System/Vector2.hpp>


class Enemy : public Ship
{
	friend class List;
	friend class Listnode;
	public:
		Enemy ();
		~Enemy ();
		void update(sf::RenderWindow & window);
		void set_formation (float form_x, float form_y);
		void set_stop (int newstop);
		sf::Vector2f move;
	protected:
		int stop;
		int explosion_timer;

		

};


#endif