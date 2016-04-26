#include "Config.h"

Config * Config::mInstance = 0;

/**
 * initialize variables
 * TODO: add support for config files
 */
//========================================
Config::Config()
{
	//--set defaults
	mScreenH = 600;
	mScreenW = 800;

	//readFile()
}

/**
 * TODO: more file support
 */
Config::~Config()
{
	//saveFile();
}

/**
 * Initialize unique Config object,
 * returns Config::mInstance
 */
//========================================
Config * Config::init()
{
	if(!Config::mInstance)
	{
		Config::mInstance = new Config;
	}
	else
	{
		//TODO: error log file output
	}
	return Config::mInstance;

}

/**
 * returns Config::mInstance
 */
//========================================
Config * Config::instance()
{
	Config * retval;

	if(!Config::mInstance)
	{
		retval = Config::init();
	}
	else
	{
		retval = Config::mInstance;
	}

	return retval;
}

/**
 * delete unique instance, sets Config::mInstance to 0
 */
//========================================
void Config::destroy()
{
	delete Config::mInstance;
	Config::mInstance = 0;
}
