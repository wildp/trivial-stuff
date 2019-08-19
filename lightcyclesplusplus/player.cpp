// player.cpp
// contains code for the Player class

#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "colours.h"
#include "directions.h"

#include "player.h"

// defaults in case setMidpoint is not called
short Player::s_playerNoGenerator	= 0;
SDL_Point Player::s_midpoint		= { 50, 50 }; 
SDL_Point Player::s_endpoint		= { 100, 100 };

void Player::setMidpoint(SDL_Point gridSize) //static
{
	s_midpoint.x = (gridSize.x / 2);
	s_midpoint.y = (gridSize.y / 2);
	s_endpoint	 = gridSize;
}

void Player::resetGenerator() //static
{
	s_playerNoGenerator = 0;
}

const SDL_Point Player::getStartLocation()
{
	switch (m_playerNo)
	{
	case 0:		return {			0,	s_midpoint.y };
	case 1:		return { s_endpoint.x,	s_midpoint.y };
	case 2:		return { s_midpoint.x,			   0 };
	case 3:		return { s_midpoint.x,	s_endpoint.y };

	default:	return { s_midpoint.x,	s_midpoint.y };
	}
}

Player::Player(): 
	m_playerNo{ s_playerNoGenerator++ },
	m_alive{ State::ISALIVE }
{
	m_dir = Game::startDir[m_playerNo];
	m_trail.reserve(s_endpoint.x * s_endpoint.y);
	m_location = getStartLocation();
	m_trail = { m_location };
}

void Player::move(Directions cdir)
{
	if (m_alive == State::ISALIVE)
	{
		switch (cdir)
		{
		case(Directions::RIGHT):	if (m_dir != Directions::LEFT)	m_dir = cdir; break;
		case(Directions::LEFT):		if (m_dir != Directions::RIGHT) m_dir = cdir; break;
		case(Directions::DOWN):		if (m_dir != Directions::UP)	m_dir = cdir; break;
		case(Directions::UP):		if (m_dir != Directions::DOWN)	m_dir = cdir; break;
		}

		switch (m_dir)
		{
		case(Directions::RIGHT):	m_location.x++; break;
		case(Directions::LEFT):		m_location.x--; break;
		case(Directions::DOWN):		m_location.y++; break;
		case(Directions::UP):		m_location.y--; break;
		}

		m_trail.push_back(m_location);
	}
}

bool Player::collide(std::vector<Player> &p, const short &PLAYERS, const bool &DELETETRAILS)
{
	if (m_alive == State::ISALIVE)
	{
		for (int player{ 0 }; player < PLAYERS; player++)
		{
			std::vector<SDL_Point> &t{ p[player].m_trail };
			int no = p[player].getPlayerNo();

			for (int i{ 0 }; i < ((no == m_playerNo) ? (t.size() - 1) : (t.size())); ++i)
				if ((m_location.x == t[i].x) && (m_location.y == t[i].y))
				{
					if (no == m_playerNo)
						std::cout << "Player " << Game::colourNames[m_playerNo] << " has committed suicide.\n";
					else
						std::cout << "Player " << Game::colourNames[m_playerNo] << " was killed by Player " << Game::colourNames[no] <<".\n";
					die(p, PLAYERS, DELETETRAILS);
					return false;
				}
		}
		if ((m_location.x < 0) || (m_location.x > s_endpoint.x) || (m_location.y < 0) || (m_location.y > s_endpoint.y))
		{
			std::cout << "Player " << Game::colourNames[m_playerNo] << " has died.\n";
			die(p, PLAYERS, DELETETRAILS);
			return false;
		}
		return true;
	}
	if (m_alive == State::NONE) die(p, PLAYERS, DELETETRAILS);
	return false;
}

void Player::die(std::vector<Player> &p, const short &PLAYERS, const bool &DELETETRAILS)
{
	if (m_alive == State::NONE)
	{
		m_alive = State::ISDEAD;

		if (DELETETRAILS) m_trail.clear();
		else
		{
			bool status{ false };

			for (int player{ 0 }; player < PLAYERS; player++)
			{
				if (p[player].getPlayerNo() != m_playerNo)
					if ((m_location.x == p[player].m_trail.back().x) && (m_location.y == p[player].m_trail.back().y))
						status = true;

			}


			if (!status) m_trail.pop_back();
		}

	}
	else m_alive = State::NONE;
}


void Player::test() { std::cout << "Player Object " << m_playerNo << "\n"; }

// is this even needed?
Player::~Player()
{
	uint64_t s{ m_trail.size() };
	for (int i{ 0 }; i < s; i++)
		m_trail.pop_back();
}