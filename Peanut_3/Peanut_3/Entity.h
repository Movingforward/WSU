#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <vector>


class Entity
{
public:
	~Entity();
	sf::Texture		texture;
	int				x;
	int				y;
	int				w;
	int				h;

	Entity(std::string textureFile)
	{
		if(!texture.loadFromFile(textureFile))
			std::cout << "Error";
		texture.setSmooth(true);
	}

};


#endif