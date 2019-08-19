#ifndef STRUCT_H
#define STRUCT_H

#include <SDL.h>

struct Data
{
	SDL_Point	GRID;
	short		SCALE;
	short		SPEED;
	short		FIRSTTO;
	short		PLAYERS;
	int 		FLASH;
	bool		NOFRILLS;
	bool		DELETETRAILS;
	bool		INFINITELOOP;
	bool		DEBUG;
};

#endif // !STRUCT_H


