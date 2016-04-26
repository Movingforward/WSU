#define PI 3.14159

#include "Menu.h"
#include <cmath>

Menu::Menu()
{
	playing = false;
	speed = 0.5;
}
int Menu::Run(sf::RenderWindow &window)
{
	bool running = true;
	selection = 0;

	Text play("play", 70), again("again", 70), settings("settings", 70), close("close", 70);

	play.setPosition	(270, 150);
	again.setPosition	(270, 150);
	settings.setPosition(270, 250);	
	close.setPosition	(270, 350);

	while (running)
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				return -1;
			}
			//keyboard selection
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					if (selection > 0)
					{
						selection -= 1;
					}
					else
					{
						selection = 0;
					}
					break;
				case sf::Keyboard::Down:
					if (selection < 2)
					{
						selection += 1;
					}
					else
					{
						selection = 2;
					}
					break;
				case sf::Keyboard::Return:
					if (selection == 0)
					{
						if (playing == true)
						{
							return 3;
						}
						else
						{
							playing = true;
							return 1;
						}
					}
					else if (selection == 1)
					{
						return 2;
					}
					else
					{
						return -1;
					}
					break;
				default:
					break;
				}
			}
		}
		if (selection == 0)//Play
		{
			play.setColor	 (sf::Color(255, 128,   0));
			again.setColor	 (sf::Color(255, 128,   0));
			settings.setColor(sf::Color(255, 255, 255));
			close.setColor	 (sf::Color(255, 255, 255));
		}
		else if (selection == 1)//Settings
		{
			play.setColor	 (sf::Color(255, 255, 255));
			again.setColor	 (sf::Color(255, 255, 255));
			settings.setColor(sf::Color(255, 128,   0));
			close.setColor	 (sf::Color(255, 255, 255));
		}
		else//Quit
		{
			play.setColor	 (sf::Color(255, 255, 255));
			again.setColor	 (sf::Color(255, 255, 255));
			settings.setColor(sf::Color(255, 255, 255));
			close.setColor	 (sf::Color(255, 128,   0));
		}
	}
}