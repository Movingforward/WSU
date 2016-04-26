#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>

class Background
{
public:
	Background(std::string filepath);
	void Render(sf::RenderWindow &window);
private:
	sf:: Texture bgTex;
	sf:: Sprite bgSprite;
};
class MovingBackground
{
public:
	MovingBackground(std::string filepath, sf::RenderWindow &window);
	void Update(sf::RenderWindow &window, float &elapsedTime, float &speed);
	void Render(sf::RenderWindow &window);
	float getY();
private:
	sf::Texture mbgTex;
	sf::Sprite mbgSprite;
	float bgSpeed;
	float bgY;
};
class Text
{
public:
	Text();
	Text(int size);
	Text(std::string output, int size);
	void setString(std::string string);
	void setSize(int size);
	void setColor(sf:: Color color);
	void setPosition(float x, float y);
	void setOrigin(float x, float y);
	void Update(std::stringstream &stream, int value);
	void Render(sf:: RenderWindow &window);
	sf::FloatRect Text::getGlobalBounds();
private:
	sf::Text text;
	sf::Font font;

};



#endif GUI_H