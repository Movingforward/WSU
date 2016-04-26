#pragma once

#include <SFML\System\Vector2.hpp>
#include <SFML\Audio.hpp>
#include <cmath>

unsigned const W_WIDTH = 1024;
unsigned const W_HEIGHT = 768;

float const PI = 3.14159265f;

#define E_AUDIO_INIT_FAILURE 1
#define E_SOUND_INIT_FAILURE 2
#define E_SHADER_INIT_FAILURE 3

typedef unsigned GameInitException;

inline float vec2fLen(sf::Vector2f v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

inline bool isOutOfBounds(sf::Vector2f pos)
{
	return (pos.x > W_WIDTH || pos.y > W_HEIGHT || pos.x < 0.0f || pos.y < 0.0f);
}

inline int randClamp(int clamp, int base = 0)
{
	return rand() % clamp + base;
}

// lol
inline float randClampf(int clamp, int base = 0)
{
	return static_cast<float>(rand() % clamp + base);
}

inline sf::Color randColor()
{
	return sf::Color(randClamp(255), randClamp(255), randClamp(255));
}

inline bool initSound(sf::SoundBuffer &buf, sf::Sound &snd, char const *fname, float vol)
{
	if(!buf.loadFromFile(fname))
		return false;

	snd.setBuffer(buf);
	snd.setVolume(vol);
	return true;
}