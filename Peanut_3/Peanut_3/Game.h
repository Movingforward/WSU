#ifndef GAME_H
#define GAME_H

class Game
{
public:
	~Game();

	static Game *		init();
	static Game *		getInstance();
	static void			destroy();

	void				newGame();
	void				nextLevel();

private:
	static Game *		mInstance;
	Game();
};

#endif //GLOBAL_H