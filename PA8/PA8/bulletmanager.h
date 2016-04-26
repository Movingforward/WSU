#pragma once

// TODO: try and merge with entity manager

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#define NO_BULLET_INTERSECTION -1
typedef int BulletID;

class Bullet : public sf::Drawable
{
	friend class BulletManager;

public:
	Bullet(sf::Vector2f pos, sf::Vector2f normalFaceDir, float vel);
	~Bullet() {}

protected:
	void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	sf::ConvexShape ent;
	sf::Vector2f faceDir;
	float vel;
};

class BulletManager
{
public:
	BulletManager();

	void addBullet(sf::Vector2f pos, sf::Vector2f faceVec, float vel, float soundMod = 0);
	void updateBullets(sf::RenderWindow &window);
	BulletID isBulletTouching(sf::FloatRect boundBox);
	bool removeBullet(BulletID id);

private:
	sf::SoundBuffer fsBuf;
	sf::Sound fireSound;

	std::vector<Bullet> bullets;
};