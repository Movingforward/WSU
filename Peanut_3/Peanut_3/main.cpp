/*  PA8 using SFML

Group Members:
Simon Ainsworth
Chris Knox
Zac Reeves
Joe Schussler

Description:  This is a simple "shoot 'em up" game using SFML.  Use the arrow keys to control the ship
			  and the space bar to shoot.

NOTE: Collision.h and collision.cpp were found on GitHub.

*/
#include "Entity.h"
#include "Ship.h"
#include "Config.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Collision.h"
#include "GUI.h"
#include "Menu.h"




#define EXIT_SUCCESS 0


void func()
{
	//this function is started when thread.Launch() is called

	for (int i = 0; i < 10; ++i)
		std::cout << "I'm thread number one" << std::endl;
}


void eventLoop()
{
};

int main()
{
	srand ((unsigned int)time (NULL));

	Ship *ship = NULL;
	std::vector <Bullet> enemy_Bullet(20);
	std::vector <Bullet> player_Bullet(10);
	Enemy E_En_arr[10][10];


	Bullet *nBullet = NULL;

	int index = 0,                current_wave = 0, 
		position_set = 0,         set_count = 0, 
		formation_is_set = 0,     select = 0, 
		newstop = 0,              all_destroyed = 0, 
		shot_timing_counter = 1,  shot_frequency = 1666, //controlls enemy shot frequency. 1 is fastest.
		enemy_bullet_select = 0,  score = 0,
		count;
	float formation_x = 0,        formation_y = 0;
	std::string score_string = "Score: ", health_string = "Health: ", cur_wave_string = "Current Wave: ";

	sf::Font font;
	font.loadFromFile ("arialbi.ttf");

	sf::Text Score;
	Score.setFont (font);
	Score.setString (score_string);
	Score.setCharacterSize (24);
	Score.setColor (sf::Color::Blue);

	sf::Text Health;
	Health.setFont (font);
	Health.setString (health_string);
	Health.setPosition (150, 0);
	Health.setCharacterSize (24);
	Health.setColor (sf::Color::Red);

	sf::Text Cur_wave;
	Cur_wave.setFont (font);
	Cur_wave.setString (cur_wave_string);
	Cur_wave.setPosition (300, 0);
	Cur_wave.setCharacterSize (24);
	Cur_wave.setColor (sf::Color::Green);


	sf::RenderWindow window;
	window.create (sf::VideoMode (800, 600), "SFML Game");
	window.setFramerateLimit(120);
	window.setKeyRepeatEnabled (false);

	ship = new Ship();

	while (window.isOpen ())
	{
		sf::Event event;
		while (window.pollEvent (event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				enemy_Bullet.clear();
				player_Bullet.clear();
				window.close();
			}
			/*
			Moving some of this to the ship class, and allowing the speed to be modified could be useful.
			*/

			else if (event.type == sf::Event::Closed)
			{
				player_Bullet.clear();
				window.close ();
			}

			if (sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
			{	
				ship->move.y = -10;
			}
			else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
			{
				ship->move.y = 10;
			}
			else
			{
				ship->move.y = 0;
			}
			if (sf::Keyboard::isKeyPressed (sf::Keyboard::Right))//don't want else, or you cannot have two directions.
			{
				ship->move.x = 10;
			}
			else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Left))
			{
				ship->move.x = -10;
			}
			else
			{
				ship->move.x = 0;
			}
		}
		index = 0;
		if (formation_is_set == 0)
		{
			select = rand () % 5;
			set_count = 0;
			switch (select)
			{
			case 0: //formation 1
				{
					set_count = 0;
					newstop = 30;
					formation_x = 150;
					formation_y = -150;
					while (index < 10)
					{
						if (set_count == 5)
						{
							formation_y = -50;
							formation_x = 150;
						}

						E_En_arr[index][current_wave].set_formation (formation_x, formation_y);

						if (index > 4)
						{
							E_En_arr[index][current_wave].set_stop (200);
						}

						set_count++;
						formation_x += 100;
						index++;
					}
					formation_is_set = 1;
					break;
				}

			case 1: //formation 2
				{
					set_count = 0;
					newstop = 30;
					formation_x = 40;
					formation_y = -150;
					while (index < 10)
					{					
						E_En_arr[index][current_wave].set_formation (formation_x, formation_y);
						E_En_arr[index][current_wave].set_stop (newstop);

						if (set_count > 4)
						{
							formation_y -= 50;
							newstop -= 30;
							formation_x += 75;
						}
						else if (set_count == 4)
						{
							formation_x += 75;
						}

						else
						{
							formation_y += 50;
							newstop += 30;
							formation_x += 75;
						}

						set_count++;
						index++;
					}
					formation_is_set = 1;
				}
				break;

			case 2: //formation 3
				{
					set_count = 0;
					newstop = 150;
					formation_x = 40;
					formation_y = -150;
					while (index < 10)
					{					
						E_En_arr[index][current_wave].set_formation (formation_x, formation_y);
						E_En_arr[index][current_wave].set_stop (newstop);

						if (set_count > 4)
						{
							formation_y -= 50;
							newstop += 30;
							formation_x += 75;
						}
						else if (set_count == 4)
						{
							formation_x += 75;
						}
						else
						{
							formation_y -= 50;
							newstop -= 30;
							formation_x += 75;
						}

						set_count++;
						index++;
					}
					formation_is_set = 1;
				}

			case 3: //formation 4
				{
					set_count = 0;
					newstop = 30;
					formation_x = 100;
					formation_y = -100;
					while (index < 10)
					{					
						E_En_arr[index][current_wave].set_formation (formation_x, formation_y);
						E_En_arr[index][current_wave].set_stop (newstop);
						newstop += 75;

						if (set_count == 4)
						{
							formation_x = 600;
							formation_y = -100;
							newstop = 30;
						}

						formation_y += 50;
						set_count++;
						index++;
					}
					formation_is_set = 1;
				}
				break;
			case 4: //formation 5
				{
					set_count = 0;
					newstop = 30;
					formation_x = 150;
					formation_y = -150;
					while (index < 10)
					{					
						if (set_count == 5)
						{
							newstop = 30;
							formation_x = 600;
							formation_y = -150;
						}
						if (index == 2)
						{
							formation_x -= 50;
						}
						if (index == 7)
						{
							formation_x += 50;
						}


						E_En_arr[index][current_wave].set_formation (formation_x, formation_y);
						E_En_arr[index][current_wave].set_stop (newstop);

						if (set_count >= 5)
						{
							formation_y -= 75;
							formation_x -= 120;
							newstop += 75;
						}
						else
						{
							formation_y += 75;
							formation_x += 120;
							newstop += 75;
						}


						set_count++;
						index++;
					}
				}
				break;
			}
			formation_is_set = 1;
		}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space) && shot_timing_counter == 1)
		{
			nBullet = new Bullet(ship->getPos());
			player_Bullet.push_back(*nBullet);
			delete nBullet;
			//If you can do this without nBullet, GREAT. DO IT.
		}


		for(count = 0; (unsigned) count < player_Bullet.size(); count++)

		{


			player_Bullet[count].update(window);

			//removes bullets that are off screen.
			if (player_Bullet[count].pos.y < 0 || player_Bullet[count].pos.y > window.getSize().y || player_Bullet[count].pos.x < 0 || player_Bullet[count].pos.x > window.getSize().x)
			{
				player_Bullet.erase(player_Bullet.begin() + count);
			}

			//checks for collision.
			for (index = 0;index < 10; index++)
			{
				if(Collision::BoundingBoxTest (player_Bullet[count].getImage(), E_En_arr[index][current_wave].getImage()) == true)
				{
					E_En_arr[index][current_wave].health--;
					score += 5;
					player_Bullet.erase(player_Bullet.begin() + count);
				}
				

			}

		}


		// enemy and enemy bullets updates
		for(index = 0; index < 10; index++)
		{
			E_En_arr[index][current_wave].update (window);
			if(Collision::BoundingBoxTest (ship->getImage(), E_En_arr[index][current_wave].getImage()) == true)
			{
				E_En_arr[index][current_wave].health = 0;
				ship->health = 0;
			}
			if ((rand() % shot_frequency) == 0)
			{
				nBullet = new Bullet(E_En_arr[index][current_wave].getPos(), *ship);
				enemy_Bullet.push_back(*nBullet);
				delete nBullet;

			}

		}

		// your ship - enemy bullet collision
		for(count = 0; (unsigned) count < enemy_Bullet.size(); count++)
		{
			if(Collision::BoundingBoxTest (enemy_Bullet[count].getImage(), ship->getImage()) == true)
			{
				ship->health--;
				enemy_Bullet.erase(enemy_Bullet.begin() + count);
			}
		}
		try
		{
			for(count = 0; (unsigned) count < enemy_Bullet.size(); count++)
			{
				enemy_Bullet[count].update(window);
				//removes bullets that are off screen.
				if(enemy_Bullet[count].pos.y < 0 || enemy_Bullet[count].pos.y > window.getSize().y || 
					enemy_Bullet[count].pos.x < 0 || enemy_Bullet[count].pos.x > window.getSize().x)
				{
					enemy_Bullet.erase(enemy_Bullet.begin() + count);
				}
			}
		}
		catch(...)
		{}

		// ship and your bullets updates
		ship->update(window);

		for(count = 0; (unsigned) count < enemy_Bullet.size(); count++)
		{
			enemy_Bullet[count].update(window);
		}

		window.display ();
		window.clear ();

		// timing settings and checks
		index = 0;
		all_destroyed = 1;
		while (index < 10)
		{
			if (E_En_arr[index][current_wave].getdestroyed () == 0)
			{
				all_destroyed = 0;
			}
			index++;
		}
		if (all_destroyed == 1)
		{
			current_wave++;
			formation_is_set = 0;
		}

		if (current_wave == 10)
		{
			player_Bullet.clear();
			enemy_Bullet.clear();
			std::cout << "You win!" << std::endl;
			window.close();
		}
		if (ship->getdestroyed () == 1)
		{
			player_Bullet.clear();
			enemy_Bullet.clear();
			std::cout << "Your ship was destroyed! You lose!" << std::endl;
			//system ("pause");//Crashes game before closing.
			window.close();
		}
		//controlls speed of player fire.
		shot_timing_counter++;
		if ((shot_timing_counter % 10) == 0)
		{
			shot_timing_counter = 1;
		}
		score_string = "Score: ";
		score_string += std::to_string (score);
		Score.setString (score_string);
		health_string = "Health: ";
		health_string += std::to_string (ship->health);
		Health.setString (health_string);
		cur_wave_string = "Current Wave: ";
		cur_wave_string += std::to_string (current_wave + 1);
		Cur_wave.setString (cur_wave_string);
		window.draw(Score);
		window.draw(Health);
		window.draw(Cur_wave);
	}



	return EXIT_SUCCESS;
}

