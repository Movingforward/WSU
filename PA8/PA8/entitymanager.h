#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "bulletmanager.h"

/*
This file is very similar to how the bullet manager works. If this game were to be
developed more I would want all managers or entities to impliment the same interface.
As it stands, don't really see the point in such a small code base.
*/

class Entity : public sf::Drawable
{
	friend class EntityManager;

public:
	Entity(sf::Vector2f pos, float vel);
	~Entity() {}

	int takeDamange();

protected:
	void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	sf::ConvexShape shape;
	sf::Vector2f vel;
	int hp;
	float rotSpeed;
	bool flyingFromEdge;
};

class EntityManager
{
public:
	EntityManager();

	void addEntity(sf::Vector2f pos);
	void updateEntities(sf::RenderWindow &window, BulletManager &bm,
						sf::Clock &clk, sf::Vector2f const &playerPos);

private:
	void computeFlockPos(Entity &e, sf::Vector2f const &playerPos);

	sf::Vector2f flyTowardsCenterOfMass(Entity &e);
	sf::Vector2f keepDistance(Entity &curEnt);
	sf::Vector2f matchVelocity(Entity &curEnt);
	sf::Vector2f encourageBounds(Entity &e);


	sf::SoundBuffer dmgBuf, explosionBuf;
	sf::Sound dmgSound, explosionSound;

	std::vector<Entity> entities;
	int numEntities;
};