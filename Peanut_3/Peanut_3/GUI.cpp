#include "GUI.h"

Background::Background(std::string filepath)
{
	bgTex.loadFromFile(filepath);
	bgTex.setSmooth(false);
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);
}
void Background::Render(sf::RenderWindow &window)
{
	window.draw(bgSprite);
}
MovingBackground::MovingBackground(std::string filepath, sf::RenderWindow &window)
{
	mbgTex.loadFromFile(filepath);
	mbgTex.setSmooth(false);
	mbgTex.setRepeated(true);
	mbgSprite.setTexture(mbgTex);
	mbgSprite.setPosition(0,0);
	bgY = 0;
	mbgSprite.setTextureRect(sf::IntRect(0, bgY, window.getSize().x, window.getSize().y));
}

void MovingBackground::Update(sf::RenderWindow &window, float &elapsedTime, float &speed)
{
	if (bgY < window.getSize().y)
	{
		bgY -= speed * elapsedTime;
	}

	else
	{
		bgY = 0;
	}
	mbgSprite.setTextureRect(sf::IntRect(0, bgY, window.getSize().x, window.getSize().y));
}
void MovingBackground::Render(sf::RenderWindow &window)
{
	window.draw(mbgSprite);
}
float MovingBackground::getY()
{
	return mbgSprite.getPosition().y;
}
Text::Text(int size)
{
	font.loadFromFile("graphics/font/PressStart2P");
	text.setFont(font);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(size);
}
Text::Text(std::string output, int size)
{
	font.loadFromFile("graphics/font/PressStart2P");
	text.setFont(font);
	text.setString(output);
	text.setStyle(sf::Text::Bold);
	text.setCharacterSize(size);
}
Text::Text()
{
	font.loadFromFile("graphics/font/PressStart2P");
	text.setFont(font);
	text.setStyle(sf::Text::Bold);
}
void Text::setString(std::string string)
{
	text.setString(string);
}
void Text::setSize(int size)
{
	text.setCharacterSize(size);
}
void Text::setColor(sf::Color color)
{
	text.setColor(color);
}
void Text::setPosition(float x, float y)
{
	text.setPosition(x,y);
}
void Text::setOrigin(float x, float y)
{
	text.setOrigin(x,y);
}
void Text::Update(std::stringstream &stream, int value)
{
	text.setString(stream.str());
	stream.str("");
	stream << value;
}
void Text::Render(sf::RenderWindow &window)
{
	window.draw(text);
}
/*sf::FloatRect Text::getGlobalBounds()
{
	return text.getGlobalBounds;
}*/
