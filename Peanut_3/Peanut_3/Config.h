
#ifndef CONFIG_H
#define CONFIG_H

#include <cstdio>

/**
* contains data obtained from config file.
*/
//=========================================
class Config
{
public:
	~Config();

	static Config *		init();
	static Config *		instance();
	static void			destroy();

	void				readValues(FILE* file);
	bool				readFile();
	bool				saveFile();

	void				setScreenSize(int m);
	void				setScreenSize(int w, int h);

	int					screenW()			{	return mScreenW;	}
	int					screenH()			{	return mScreenH;	}

private:
	int					mScreenW;
	int					mScreenH;

	static Config *		mInstance;

	Config();
};

#endif //CONFIG_H
