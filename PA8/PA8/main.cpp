#include <SFML/Graphics.hpp>

#include "game.h"
#include "util.h"
#include "musicstream.h"

char const *gameTitle = "ACIDROIDS";
PWCHAR const streamURL = L"http://somafm.com/wma128/groovesalad.asx";


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR cmdList, int cmdShow)
{
	try
	{
		MusicStream stream(streamURL);
		Game game(W_WIDTH, W_HEIGHT);

		game.play();
	}
	catch(...)
	{
		MessageBox(0, L"The game was unable to initialize.", L"Error:", MB_OK | MB_ICONWARNING);
	}

	return 0;
}