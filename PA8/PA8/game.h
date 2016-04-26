#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "bulletmanager.h"
#include "player.h"
#include "util.h"

class Game
{
public:
	Game(unsigned width, unsigned height)
		: backSurface(sf::Vector2f(static_cast<float>(width), static_cast<float>(height))),
		player(sf::Vector2f(W_WIDTH / 2, W_HEIGHT / 2))
	{}

	void play();

private:
	sf::RectangleShape backSurface;
	sf::Shader shader;

	Player player;
	BulletManager bManager;
};