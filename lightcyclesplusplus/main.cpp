// main.cpp
// sets up SDL, starts game

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <sstream>

#include "struct.h"
#include "log.h"
#include "game.h"

int extractInt(char c[], int default,  bool enableZero = false, bool oddonly = false)
{	
	// extract an int from a string with format "-x??" where ? is an int and x is a char
	c[0] = '0'; c[1] = '0'; // cannot extract if there are symbols before the numbers
	std::stringstream convert(c);
	int myint{ default };
	if (!(convert >> myint))
		myint = default;
	if (myint <= 0)
		myint = default;

	if (oddonly)
		if (myint % 2 == 1)
			++myint;

	return myint;
}

short extractLimitedInt(char c[], int default, int minimum, int maximum)
{
	// extract an int from a string with format "-x??" where ? is an int and x is a char
	c[0] = '0'; c[1] = '0'; // cannot extract if there are symbols before the numbers
	std::stringstream convert(c);
	int myint{ default };
	if (!(convert >> myint))
		myint = default;
	std::cout << myint << ' ';

	// bounds checking for int
	if (myint < minimum) myint = default;
	if (myint > maximum) myint = default;;
	return myint;
}

bool extractBool(char c[], int default)
{
	// extract a bool from a string with format "-x??" where ? is an int and x is a char
	c[0] = '0'; c[1] = '0'; // cannot extract if there are symbols before the numbers
	if (c[2] == '\0') return true;
	std::stringstream convert(c);
	int myint{ default };
	if (!(convert >> myint))
		myint = default;
	if ((myint != 0) && (myint != 1))
		myint = default;

	return static_cast<bool>(myint);
}

SDL_WindowFlags decode(int i)
{
	switch (i)
	{
	case 0:		return SDL_WINDOW_SHOWN;
	case 1:		return SDL_WINDOW_BORDERLESS;
	case 2:		return SDL_WINDOW_FULLSCREEN;
	default:	return SDL_WINDOW_SHOWN;
	}
}

//#undef main // SDL injects with its own main, so undefine main before my own main

int main(int argc, char *argv[])
{	
	/**	Command Line Arguments (prefixed with -), (alternative defaults listed)
	* -x[uint] set size of GRID.x				(default 100)	(-x144)	(-x120)	(-x42)
	* -y[uint] set size of GRID.y				(default 100)	(-y144)	(-x120)	(-y42)
	* -z[uint] set size of both GRID.x and GRID.y at same time
	* -g[uint] set grid SCALE					(default 8)		(-g6)	(-g8)	(-g20)
	* -s[uint] set frame time					(default 30)	(-s10)	(-s20)	(-s50)
	* -w[uint] set number of games to play to	(default 3)
	* -p[uint] set number of players (max 4)	(default 4)
	* -f[UINT] set FLASH countdown timer		(default 0)
	* -n[bool] enable NOFRILLS mode				(default 0)
	* -d[bool] set DELETETRAILS (1 or 0)		(default 0)
	* -l[bool] set INFINITELOOP (1 or 0)		(default 0)
	* -m[uint] set WINDOWMODE (enum)			(default 0)
	* -a[uint] set autoTarget					(default 10)
	*
	* @ to enable Fullscreen auto settings
	* # to enable DEBUG mode
	*/

	// Setup Important Variables
	Data d{};
	d.GRID					= { 100, 100 };
	d.SCALE					= 8;
	d.SPEED					= 30;
	d.FIRSTTO				= 3;
	d.PLAYERS				= 4;
	d.FLASH					= 0;
	d.NOFRILLS				= false;
	d.DELETETRAILS			= false;
	d.INFINITELOOP			= false;

	d.DEBUG = false;

	int WINDOWMODE{ 0 };
	int autoTarget { 10 };
	bool autoSettings{ false };

	char* PATH[]{ SDL_GetBasePath() };
	if (PATH == nullptr)
	{	// check if PATH exists
		logSDLError("SDL_GetBasePath");
		return 1;
	}

	// Deal with Command Line Arguments

	for (int i = 0; i < argc; ++i)
	{
		if (argv[i][0] == '@') autoSettings = true;
		if (argv[i][0] == '#') d.DEBUG = true;
		if (argv[i][0] == '-')
		{	// Ignore all non legitimate arguments
			switch (argv[i][1])
			{
			case 'x':
				d.GRID.x = extractInt(argv[i], d.GRID.x, true); break;
			case 'y':
				d.GRID.y = extractInt(argv[i], d.GRID.y, true); break;
			case 'z':
				d.GRID.x = extractInt(argv[i], d.GRID.x, true);
				d.GRID.y = extractInt(argv[i], d.GRID.y, true); break;
			case 'g':
				d.SCALE = extractInt(argv[i], d.SCALE); break;
			case 's':
				d.SPEED = extractInt(argv[i], d.SPEED); break;
			case 'w':
				d.FIRSTTO = extractInt(argv[i], d.FIRSTTO); break;
			case 'p':
				d.PLAYERS = extractLimitedInt(argv[i], d.PLAYERS, 1, 4); break;
			case 'f':
				d.FLASH = extractInt(argv[i], d.FLASH); break;
			case 'n':
				d.NOFRILLS = extractBool(argv[i], d.NOFRILLS); break;
			case 'd':
				d.DELETETRAILS = extractBool(argv[i], d.DELETETRAILS); break;
			case 'l':
				d.INFINITELOOP = extractBool(argv[i], d.INFINITELOOP); break;
			case 'm':
				WINDOWMODE = extractLimitedInt(argv[i], WINDOWMODE, 0, 3); break;
			case 'a':
				autoTarget = extractInt(argv[i], d.FLASH); break;
			}
		}
	}

	SDL_Point SCREEN{ (d.GRID.x + 1) * d.SCALE, (d.GRID.y + 1) * d.SCALE };

	if (!autoSettings) logSettings(d);

	// SDL Setup

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{	// Check if SDL has initialised
		logSDLError("SDL_Init");
		return 1;
	}

	if (TTF_Init() != 0)
	{
		logSDLError("TTF_Init");
		SDL_Quit();
		return 1;
	}

	// do automatic settings

	if (autoSettings)
	{
		SDL_DisplayMode DM;
		SDL_GetDesktopDisplayMode(0, &DM);
		SCREEN.x = DM.w;
		SCREEN.y = DM.h;

		std::cout << "SCREEN " << SCREEN.x << 'x' << SCREEN.y << '\n';

		
		d.SCALE = autoTarget;
		while (((SCREEN.x % d.SCALE) != 0) && ((SCREEN.y % d.SCALE) != 0)) 
			--d.SCALE;

		d.GRID = { (SCREEN.x / d.SCALE), (SCREEN.y / d.SCALE) };
		WINDOWMODE = 2;

		logSettings(d);
	}

	// SDL Setup Continued

	SDL_Window *window = SDL_CreateWindow("Lightcycles++", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN.x, SCREEN.y, decode(WINDOWMODE));

	if (window == nullptr)
	{	// Check if a window has been created
		logSDLError("CreateWindow");
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == nullptr)
	{	// Check if a renderer has been created
		logSDLError("CreateRenderer");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

	// Actual Main

	do
	{
		d.INFINITELOOP = game(window, renderer, d, SCREEN, *PATH);

	}while(d.INFINITELOOP);

	// Cleanup

	std::cout << "\n\nExiting...\n";
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();

	return 0;
}