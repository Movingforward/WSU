#include <SFML/Audio.hpp>
#include <cmath>

#include "bulletmanager.h"
#include "util.h"

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f normalFaceDir, float vel)
	: faceDir(normalFaceDir), vel(vel)
{
	ent.setPointCount(4);
	ent.setPoint(0, sf::Vector2f(0, 0));
	ent.setPoint(1, sf::Vector2f(0, 6));
	ent.setPoint(2, sf::Vector2f(4, 6));
	ent.setPoint(3, sf::Vector2f(4, 0));
	ent.setOrigin(2, 3);

	ent.setFillColor(sf::Color(0, 242, 255));
	ent.setOutlineColor(sf::Color(0, 0, 0));
	ent.setOutlineThickness(2);
	ent.setPosition(pos);

	float angle = acos(normalFaceDir.y) * 180.0f / PI;
	if(normalFaceDir.x > 0.0f)
		angle = -angle;

	ent.setRotation(angle);
}

void Bullet::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(ent, states);
}

BulletManager::BulletManager()
{
	if(!initSound(fsBuf, fireSound, "fire.wav", 30.0f))
		throw E_SOUND_INIT_FAILURE;

	bullets.reserve(50);
}

void BulletManager::addBullet(sf::Vector2f pos, sf::Vector2f faceVec, float vel, float soundMod)
{
	//float const pitchThresh = 10.0f;
	if(!isOutOfBounds(pos))
	{
		fireSound.play();
		bullets.push_back(Bullet(pos, faceVec, vel));
	}
}

void BulletManager::updateBullets(sf::RenderWindow &window)
{
	for(auto it = bullets.begin(); it < bullets.end();)
	{
		Bullet &b = *it;
		sf::Vector2f pos = b.ent.getPosition();

		if(isOutOfBounds(pos))
		{
			// erase returns new iterator pointing to element after deleted one
			it = bullets.erase(it);
		}
		else
		{
			b.ent.move(b.faceDir * b.vel);
			window.draw(b);
			++it;
		}
	}
}

BulletID BulletManager::isBulletTouching(sf::FloatRect boundBox)
{
	size_t id = 0;
	for(auto it = bullets.begin(); it < bullets.end(); ++it, ++id)
	{
		Bullet &b = *it;
		if(boundBox.intersects(b.ent.getGlobalBounds()))
		{
			return id;
		}
	}
	return NO_BULLET_INTERSECTION;
}

bool BulletManager::removeBullet(BulletID id)
{
	bullets.erase(bullets.begin() + id);
	return true;
}
