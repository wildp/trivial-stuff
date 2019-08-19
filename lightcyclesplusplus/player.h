// player.h
// contains declarations for the Player class

#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "directions.h"

class Player
{
	static short s_playerNoGenerator;
	static SDL_Point s_midpoint;
	static SDL_Point s_endpoint;

	const short m_playerNo;

	SDL_Point m_location;
	Directions m_dir;

	enum class State
	{
		ISALIVE,
		ISDEAD,
		NONE,
	};

	State m_alive;

	const SDL_Point getStartLocation();

	void die(std::vector<Player> &p, const short &PLAYERS, const bool &DELETETRAILS);

public:
	std::vector<SDL_Point> m_trail;

	static void setMidpoint(SDL_Point gridSize);

	static void resetGenerator();

	Player();
	
	void move(Directions cdir);

	bool collide(std::vector<Player> &p, const short &PLAYERS, const bool &DELETETRAILS);

	const int getPlayerNo() { return m_playerNo; }

	void test();

	~Player();
};

#endif // !PLAYER_H

