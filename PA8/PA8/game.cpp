#include <SFML/OpenGL.hpp>
#include <cstdlib>
#include <ctime>

#include "game.h"
#include "bulletmanager.h"
#include "entitymanager.h"

const std::string vs = \
"void main()"
"{"
"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
"gl_FrontColor = gl_Color;"
"}";

const std::string fs = \
"precision mediump float;"
"uniform float clk;"		// game clock
"uniform float fire;"		// player velocity
"uniform float colorMod;"	// color modifier based on how long the player shoots
"void main()"
"{"
"float PI = 3.1415926535897932384626433832795;"
"vec4 coord = 0.005 * gl_FragCoord;"
"float v = 0.0; vec2 c = coord.xy;"
"v = sin(coord.x + clk) + cos(colorMod)*sin(coord.y + clk)/2.0 + sin(coord.x + coord.y + clk)/2.0;"
"c += vec2(sin(clk / 3.0), cos(clk / 2.0));"
"v += 0.5 * sin(colorMod + sqrt(c.x*c.x + c.y*c.y + 1.0) + mod(colorMod, clk));"
"vec3 color = vec3(sin(v*PI + colorMod), fire*sin(PI*v + 2*PI/3) + cos(PI*v + 4*PI/3), fire*sin(PI*v + 4*PI/3));"
"gl_FragColor = vec4(color*.5 + .5, 1);"
"}";

size_t playerScore = 0;	// ugly

struct InputState
{
	bool leftMouse, rightMouse;
};

sf::CircleShape crosshair(2);

static void drawCrosshair(sf::RenderWindow &window)
{
	crosshair.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
	window.draw(crosshair);
}

static InputState handleEvents(sf::RenderWindow &window)
{
	bool lostFocus = false;
	InputState state = {0};

	sf::Event event;
	while(window.pollEvent(event) || lostFocus)
	{
		if(lostFocus)
			Sleep(10);	// spinlock until focused again

		switch(event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if(event.key.code == sf::Keyboard::Escape)
					window.close();
				break;

			case sf::Event::LostFocus:
				lostFocus = true;
				window.setMouseCursorVisible(true);
				continue;
			case sf::Event::GainedFocus:
				lostFocus = false;
				window.setMouseCursorVisible(false);
				break;
		}
	}

	state.leftMouse = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	state.rightMouse = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);

	return state;
}

static void init(sf::RenderWindow &window)
{
	window.setMouseCursorVisible(false);
	window.setVerticalSyncEnabled(true);

	crosshair.setFillColor(sf::Color(0, 0, 0, 0));
	crosshair.setOutlineColor(sf::Color(0, 0, 0));
	crosshair.setOutlineThickness(2.0f);

	srand(static_cast<unsigned>(time(0)));
}


void Game::play()
{
	extern char const *gameTitle;

	sf::RenderWindow window(sf::VideoMode(W_WIDTH, W_HEIGHT, 32), gameTitle,
							/*sf::Style::Titlebar | sf::Style::Close*/ 0,
							sf::ContextSettings(24, 8, 4, 2, 0)); // turn on antialiasing

	init(window);
	EntityManager entManager;
	//entManager.addEntity(sf::Vector2f(50, 50));
	//entManager.addEntity(sf::Vector2f(100, 50));
	//entManager.addEntity(sf::Vector2f(150, 50));

	if(!shader.loadFromMemory(vs, fs))
		throw E_SHADER_INIT_FAILURE;

	sf::Clock clk;
	while(window.isOpen())
	{
		InputState state = handleEvents(window);

		shader.setParameter("clk", clk.getElapsedTime().asSeconds());
		shader.setParameter("fire", player.getVel());
		shader.setParameter("colorMod", player.getFireMod());
		window.draw(backSurface, &shader);

		bManager.updateBullets(window);
		entManager.updateEntities(window, bManager, clk, player.getPos());

		player.update(window, bManager, clk.getElapsedTime(),
					  state.leftMouse, state.rightMouse);
		window.draw(player);

		drawCrosshair(window);
		window.display();
	}
}

