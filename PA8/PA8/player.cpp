#include <cmath>

#include "player.h"
#include "bulletmanager.h"
#include "util.h"

Player::Player(sf::Vector2f position): playerVel(0.0), fireMod(0.0), isHit(false)
{
	if(!initSound(hurtBuf, hurtSound, "ouch.wav", 95.0f) ||
	   !initSound(rocketBuf, rocketSound, "rocketloop.wav", 70.0f))
	   throw E_SOUND_INIT_FAILURE;

	rocketSound.setLoop(true);

	playerT.setPointCount(3);
	playerT.setPoint(0, sf::Vector2f(0, 0));
	playerT.setPoint(1, sf::Vector2f(6, 15));
	playerT.setPoint(2, sf::Vector2f(12, 0));
	playerT.setOrigin(6, 6);

	playerT.setFillColor(sf::Color(0, 0, 0));
	playerT.setOutlineColor(sf::Color(255, 255, 255));
	playerT.setOutlineThickness(2);
	playerT.setPosition(position);
}


// https://o3xn.wordpress.com/2015/01/24/programming-binding-object-rotation-to-mouse-movement/
sf::Vector2f Player::faceMouse(sf::Vector2f pos, sf::Vector2f mPos)
{
	sf::Vector2f q = sf::Vector2f(mPos) - pos;
	q /= vec2fLen(q);

	float angle = acos(q.y) * 180.0f / PI;

	if(q.x >= 0.0f)
		angle = -angle;

	playerT.setRotation(angle);

	return q;
}

void Player::update(sf::RenderWindow const &window, BulletManager &bManager,
					sf::Time &time, bool isThrusting, bool isShooting)
{
	static sf::Time prevTime;
	//extern size_t playerEnergy;

	sf::Vector2f pos = playerT.getPosition();
	sf::Vector2f mPos = sf::Vector2f(sf::Mouse::getPosition(window));
	sf::Vector2f faceDir;

	if(isShooting)
		fireMod += 0.03f;
	if(fireMod > 0.02f)
		fireMod -= 0.02f;
	else
		fireMod = 0.00f;

	if(vec2fLen(mPos - pos) > 20.0f)
	{
		faceDir = faceMouse(pos, mPos);

		if(isThrusting && playerVel < 8.0f)
		{
			playerVel += 0.2f;

			// fix this, not looping properly
			/*
			if(rocketSound.getStatus() == sf::SoundSource::Status::Stopped)
			rocketSound.play();
			*/
		}
		else if(playerVel > 0.5f)
		{
			playerVel -= 0.05f;
			//rocketSound.stop();
		}

		playerT.move(faceDir * playerVel);
		if(isHit)
			isHit = false;
	}
	else
	{
		// player 'hit' the crosshair
		if(!isHit)
		{
			hurtSound.play();
			playerVel = 0.0;
			//playerEnergy -= 25;
			isHit = true;
		}

		return;
	}

	if(isShooting && (time - prevTime).asSeconds() > 0.20f)
	{
		bManager.addBullet(playerT.getPosition(), faceDir, 20.0f, fireMod);
		//--playerEnergy;
		prevTime = time;
	}

	// die
	/*
	if(playerEnergy <= 0)
	{
	//playerVel = 0.0;
	}
	*/
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(playerT, states);
}