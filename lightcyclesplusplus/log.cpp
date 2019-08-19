// log.cpp
// holds functions for logging errors and the game

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "struct.h"

void logSDLError(std::ostream &os, const std::string &msg)
{
	// display an error message to an ostream of choice (normally std::cout)
	os << msg << " error: " << SDL_GetError() << std::endl;
	system("pause"); //leave this in so that message can be seen
}

void logSDLError(const std::string &msg) { logSDLError(std::cerr, msg); }

void SDLError(const std::string &msg, SDL_Window *win, SDL_Renderer *ren)
{
	logSDLError(msg);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();
	exit(1);
}

void logSettings(std::ostream &os, const Data &d)
{
	std::cout << "GRID " << d.GRID.x << ' ' << d.GRID.y << '\n' << "SCALE " << d.SCALE << '\n' << "SPEED " << d.SPEED << '\n' << "FIRSTTO " << d.FIRSTTO << '\n' << "PLAYERS " << d.PLAYERS << '\n' << "FLASH " << d.FLASH << '\n' << "NOFRILLS " << d.NOFRILLS << '\n' << "DELETETRAILS " << d.DELETETRAILS << '\n' << "INFINITELOOP " << d.INFINITELOOP << '\n' << "DEBUG " << d.DEBUG << '\n' << std::endl;
}

void logSettings(Data &d) { logSettings(std::cout, d); }