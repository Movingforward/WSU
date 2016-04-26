#include "Enemy.h"

/*
	Enemy Constructors
	Description: Below is the constructor for the Enemy class.			 
	NOTE: Enemy inherits from the Ship class.
*/
Enemy:: Enemy ()
{
	texture.loadFromFile("enemy2.png");

	shipImage.setTexture(texture);
	shipImage.setPosition(100,500);
	pos = shipImage.getPosition();
	move.x = 0;
	move.y = 0;
	health = 10;
	stop = 100;
	explosion_timer = 0;
}

Enemy::~Enemy ()
{}
/*
	void Enemy::update(sf::RenderWindow & window, int explosion_timing)
	Description: This function updates the necessary values for the
				 Enemy ships so they act as intended during gameplay.			 
*/
void Enemy::update (sf::RenderWindow & window)
{
	float ship_size = 64;
	int exp_disp = 0;
	//Needs to check if hit
	//Needs to check if it will move outside screen.
	if (pos.y + move.y > stop)
	{
		move.y = 0;
	}
	else
	{
		move.y = (float) .5;
	}

	if (health <= 0 && destroyed == 0)
	{
		destroyed = 1;
		//shipImage.setPosition (-200,-200);
		texture.loadFromFile ("explosion.jpg");
		shipImage.setTexture (texture);
		
	}
	else if (health <= 0 && destroyed == 1)
	{
		explosion_timer++;
		if(explosion_timer == 30)
		{
			shipImage.setPosition (-200,-200);
		
		}
	}
	


	
	shipImage.move(move);
	pos += move;

	window.draw(shipImage);
}
/*
	Certain Setters & Getters
	Description: Below are necessary getters and setters for
				 certain class members.
*/
void Enemy::set_formation (float form_x, float form_y)
{
	pos.x = form_x;
	pos.y = form_y;
	shipImage.setPosition (pos.x, pos.y);
	
}

void Enemy::set_stop (int newstop)
{
	stop = newstop;
}

