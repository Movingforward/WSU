#include <cmath>
#include <cstdlib>
#include <SFML/OpenGL.hpp>
#include "entitymanager.h"
#include "bulletmanager.h"
#include "util.h"


Entity::Entity(sf::Vector2f pos, float initVel = 0.0)
	: hp(3), vel(vel), flyingFromEdge(true),
	rotSpeed(static_cast<float>(randClamp(5, 1)))
{
	shape.setPointCount(4);
	shape.setPoint(0, sf::Vector2f(0, 0));
	shape.setPoint(1, sf::Vector2f(0, 14));
	shape.setPoint(2, sf::Vector2f(14, 14));
	shape.setPoint(3, sf::Vector2f(14, 0));
	shape.setOrigin(7, 7);

	shape.setFillColor(randColor());
	shape.setOutlineColor(sf::Color(0, 0, 0));
	shape.setOutlineThickness(3);
	shape.setPosition(pos);

	vel = sf::Vector2f(W_WIDTH / 2.0f, W_HEIGHT / 2.0f) - shape.getPosition();
	vel = (vel / vec2fLen(vel)) * initVel;


	if(randClamp(2))
		rotSpeed = -rotSpeed;
}

int Entity::takeDamange()
{
	sf::Color outline = shape.getOutlineColor();
	shape.setOutlineColor(outline + sf::Color(85, 0, 0));
	return --hp;
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(shape, states);
}

EntityManager::EntityManager()
{

	if(!initSound(dmgBuf, dmgSound, "dmg.wav", 80.0f) ||
	   !initSound(explosionBuf, explosionSound, "boom.wav", 80.0f))
	   throw E_SOUND_INIT_FAILURE;

	size_t const initialEntities = 75;
	entities.reserve(initialEntities * 2);

	sf::Vector2f pos;
	for(size_t i = 0; i < initialEntities; ++i)
	{
		// select a random wall and then do a slight offset offscreen from that to get 
		// initial entity position. this allows entities to spawn offscreen and then 
		// naturally flow into the center of the screen rather than suddently appearing.

		float const wallOffset = 25.0f;
		int wall = randClamp(4);

		// left boundary
		if(wall == 0)
			pos = sf::Vector2f(-wallOffset, randClampf(W_HEIGHT));
		// right boundary
		else if(wall == 1)
			pos = sf::Vector2f(W_WIDTH + wallOffset, randClampf(W_HEIGHT));
		// top boundary
		else if(wall == 2)
			pos = sf::Vector2f(randClampf(W_WIDTH), -wallOffset);
		// bottom boundary
		else
			pos = sf::Vector2f(randClampf(W_WIDTH), W_HEIGHT + wallOffset);

		entities.push_back(Entity(pos, 1.0f));
		++numEntities;
	}
}

void EntityManager::addEntity(sf::Vector2f pos)
{
	entities.push_back(Entity(pos));
}

void EntityManager::updateEntities(sf::RenderWindow &window, BulletManager &bm,
								   sf::Clock &clk, sf::Vector2f const &playerPos)
{
	for(auto it = entities.begin(); it < entities.end();)
	{
		Entity &e = *it;
		sf::Vector2f pos = e.shape.getPosition();

		BulletID id = bm.isBulletTouching(e.shape.getGlobalBounds());
		if(id == NO_BULLET_INTERSECTION)
		{
			computeFlockPos(e, playerPos);
			window.draw(e);
			++it;
		}
		else
		{
			bm.removeBullet(id);
			dmgSound.play();
			if(e.takeDamange() <= 0)
			{
				it = entities.erase(it);
				--numEntities;
				explosionSound.play();

				extern size_t playerScore;
				++playerScore;
			}
		}
	}
}


// shitty flocking code

// this is completely unoptimized I dont know how it's going to affect lower-end cpus... 
// hopefully 60 fps isnt too hard to meet. if everything is fine there's no need to 
// do much work making things fast where theres no real reason to.

void EntityManager::computeFlockPos(Entity &e, sf::Vector2f const &playerPos)
{
	bool outOfBounds = isOutOfBounds(e.shape.getPosition());

	//e.shape.move(e.vel);
	//return;

	if(e.flyingFromEdge && !outOfBounds)
	{
		e.flyingFromEdge = false;
	}
	else if(outOfBounds && e.flyingFromEdge)
	{
		e.shape.move(e.vel);
	}
	else
	{
		e.shape.rotate(e.rotSpeed);

		sf::Vector2f v1 = flyTowardsCenterOfMass(e);
		sf::Vector2f v2 = keepDistance(e);
		sf::Vector2f v3 = matchVelocity(e);

		sf::Vector2f towardPlayer = (playerPos - e.shape.getPosition()) / 800.0f;

		e.vel += v1 + v2 + v3 + towardPlayer + encourageBounds(e);
		e.shape.move(e.vel);
	}
}

sf::Vector2f EntityManager::flyTowardsCenterOfMass(Entity &curEnt)
{
	sf::Vector2f center(0.0f, 0.0f);

	for(auto it = entities.begin(); it < entities.end(); ++it)
	{
		Entity &e = *it;
		if(e.shape.getPosition() != curEnt.shape.getPosition())
			center += e.shape.getPosition();
	}
	center /= static_cast<float>(numEntities - 1);

	return (center - curEnt.shape.getPosition()) / 1000.0f;
}

sf::Vector2f EntityManager::keepDistance(Entity &curEnt)
{
	sf::Vector2f c(0.0f, 0.0f);
	sf::Vector2f curPos = curEnt.shape.getPosition();

	for(auto it = entities.begin(); it < entities.end(); ++it)
	{
		Entity &e = *it;
		sf::Vector2f pos = e.shape.getPosition();
		sf::Vector2f dif = pos - curPos;

		if(vec2fLen(dif) < 10.0f)
			c -= dif;
	}

	return c;
}

sf::Vector2f EntityManager::matchVelocity(Entity &curEnt)
{
	sf::Vector2f vel(0.0f, 0.0f);
	for(auto it = entities.begin(); it < entities.end(); ++it)
	{
		Entity &e = *it;
		if(e.shape.getPosition() != curEnt.shape.getPosition())
		{
			vel += e.vel;
		}
	}

	vel /= static_cast<float>(numEntities - 1);
	return (vel - curEnt.vel) / 2.0f;
}

sf::Vector2f EntityManager::encourageBounds(Entity &e)
{
	sf::Vector2f retVec;
	sf::Vector2f pos = e.shape.getPosition();

	float const retSpeed = 2.0f;
	if(pos.x < 0)
		retVec.x = retSpeed;
	else if(pos.x > W_WIDTH)
		retVec.x = -retSpeed;
	if(pos.y < 0)
		retVec.y = retSpeed;
	else if(pos.y > W_HEIGHT)
		retVec.y = -retSpeed;

	return retVec;
}