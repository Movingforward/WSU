#pragma once

#include <SFML/Graphics.hpp>
#include "bulletmanager.h"

class Player : public sf::Drawable
{
	float const playerRad = 8.0;

public:
	Player(sf::Vector2f position = sf::Vector2f((float)640 / 2, (float)480 / 2));
	Player::~Player() {}

	void update(sf::RenderWindow const &window, BulletManager &bManager,
				sf::Time &time, bool isThrusting, bool isShooting);

	float getVel() const { return playerVel; }
	float getFireMod() const { return fireMod; }
	sf::Vector2f const &getPos() const { return playerT.getPosition(); }

protected:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	sf::Vector2f faceMouse(sf::Vector2f pos, sf::Vector2f mPos);

	float playerVel, fireMod;
	sf::ConvexShape playerT;

	bool isHit;
	sf::SoundBuffer hurtBuf, rocketBuf;
	sf::Sound hurtSound, rocketSound;
};