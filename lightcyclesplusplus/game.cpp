// game.cpp
// contains the game related stuff

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

#include "struct.h"
#include "directions.h"
#include "keymap.h"
#include "colours.h"
#include "log.h"

#include "player.h"
#include "text.h"

const SDL_Rect convertToRect(const SDL_Point &coord, const int &SCALE)
{
	return { coord.x * SCALE, coord.y * SCALE, SCALE, SCALE };
}

void determineWinText(const std::vector<bool> &alive, const short &aliveCount, std::string &text, std::vector<int> &roundWins, const short &PLAYERS)
{
	if (text == " ")
	{
		//static long long count{ 0 };
		//std::cout << "count: " << count++ << " alivecount: " << aliveCount << '\n';

		if (aliveCount == 0)
		{
			text = "No One Wins";
			std::cout << "\nROUND OVER\nNo one has won the round.\n";
		}
		else if (aliveCount == 1)
			for (int player{ 0 }; player < PLAYERS; player++)
				if (alive[player] == true)
				{
					text = Game::colourNames[player] + " Wins";
					std::cout << "\nROUND OVER\nPlayer " << Game::colourNames[player] <<
						" has won the round.\n\n";
					roundWins[player]++;
				}
	}
	return; // make no changes to text 
}

bool checkGameEnd(const std::vector<int> &roundWins, const int &FIRSTTO, const int &PLAYERS)
{
	for (int player{ 0 }; player < PLAYERS; player++)
	{
		if (roundWins[player] >= FIRSTTO)
		{
			std::cout << "\n\n\nGAME OVER\nPlayer " << Game::colourNames[player] <<
				" has won the game.\n\n";
			//std::cout << "__FALSE__";
			return false;
		}
	}
	//std::cout << "__TRUE__";
	return true;
}

short updateAliveCount(std::vector<bool> &alive)
{
	return static_cast<short>(std::count(alive.begin(), alive.end(), true));
}

bool gameStart(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN)
{
	SDL_Event e{};

	Text title{ win, ren, "Lightcycles", Text::TEXT_VICTORY };
	Text centre{ win, ren, ("[Playing to " + std::to_string(d.FIRSTTO) + " wins]"), Text::TEXT_WINS , {128, 128, 128} };
	Text start{ win, ren, "Press Space to Start Game", Text::TEXT_START };

	if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
		SDLError("SetRenderDrawBlendMode", win, ren);

	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			// If user closes the window
			if (e.type == SDL_QUIT)
				return false;

			// If user presses a key
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return false;

				if (e.key.keysym.sym == SDLK_SPACE)
					return true;
			}
		}

		// Render background
		if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
			SDLError("SetRenderDrawBlendMode", win, ren);

		if (SDL_SetRenderDrawColor(ren, Game::background[0], Game::background[1], Game::background[2], SDL_ALPHA_OPAQUE))
			SDLError("SetRenderDrawColor", win, ren);

		if (SDL_RenderFillRect(ren, NULL))
			SDLError("RenderFillRext", win, ren);

		title.render(win, ren);
		centre.render(win, ren);
		start.render(win, ren);

		SDL_RenderPresent(ren);
		SDL_Delay(d.SPEED);
		SDL_RenderClear(ren);
	}
}

bool gameRound(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN, const std::string PATH, std::vector<int> &roundWins)
{
	// Setup here

	SDL_Event e{};
	Player::resetGenerator();
	Player::setMidpoint(d.GRID);
	bool pause{ false };

	std::vector<Directions> cd(d.PLAYERS);
	std::vector<Directions> new_cd(d.PLAYERS);

	std::vector<Player> pl(d.PLAYERS);

	std::vector<bool> alive(d.PLAYERS);

	std::vector<int> countDown(d.PLAYERS);

	for (int player{ 0 }; player < d.PLAYERS; player++)
	{
		cd[player] = Game::startDir[player];
		new_cd[player] = Directions::NONE;
		alive[player] = true;
		countDown[player] = d.FLASH;
	}

	bool quit{ false };
	std::string winStr{ " " };

	short aliveCount{};
	aliveCount = updateAliveCount(alive);

	Text pauseText{ win, ren, "PAUSED", Text::TEXT_PAUSE };
	Text winText{ win, ren };
	Text continueText{ win, ren, "Press Space to Continue", Text::TEXT_CONTINUE };

	std::cout << "\nROUND STARTED\n\n";

	// // Main loop

	while (!quit)
	{
		// // Grab user input

		while (SDL_PollEvent(&e))
		{
			// If user closes the window
			if (e.type == SDL_QUIT)
				return false;

			// If user presses a key
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return false;

				if (e.key.keysym.sym == SDLK_BACKSPACE)
					pause = (!pause) ? (true) : (false);

				if (e.key.keysym.sym == SDLK_SPACE)
					if (aliveCount <= 1) quit = true;

				for (int player{ 0 }; player < d.PLAYERS; player++)
				{
					if ((e.key.keysym.scancode == Game::keymap[player][0]) && (cd[player] != Directions::UP))
						new_cd[player] = Directions::UP;

					if ((e.key.keysym.scancode == Game::keymap[player][1]) && (cd[player] != Directions::LEFT))
						new_cd[player] = Directions::LEFT;

					if ((e.key.keysym.scancode == Game::keymap[player][2]) && (cd[player] != Directions::DOWN))
						new_cd[player] = Directions::DOWN;

					if ((e.key.keysym.scancode == Game::keymap[player][3]) && (cd[player] != Directions::RIGHT))
						new_cd[player] = Directions::RIGHT;
				}
			}
		}

		//

		

		for (int player{ 0 }; player < d.PLAYERS; player++)
		{
			if (new_cd[player] != Directions::NONE) cd[player] = new_cd[player];
			//std::cout << static_cast<int>(new_cd[player]) << "  " << static_cast<int>(cd[player]) << '\n';
			new_cd[player] = Directions::NONE;
		}

		// // Update player objects

		if (!pause)
		{
			for (int player{ 0 }; player < d.PLAYERS; player++)
				pl[player].move(cd[player]);

			for (int player{ 0 }; player < d.PLAYERS; player++)
				alive[player] = pl[player].collide(pl, d.PLAYERS, d.DELETETRAILS);

			aliveCount = updateAliveCount(alive);
		}

		// // Render the scene

		// Render Background
		if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
			SDLError("SetRenderDrawBlendMode", win, ren);

		if (SDL_SetRenderDrawColor(ren, Game::background[0], Game::background[1], Game::background[2], SDL_ALPHA_OPAQUE))
			SDLError("SetRenderDrawColor", win, ren);

		if (SDL_RenderFillRect(ren, NULL))
			SDLError("RenderFillRext", win, ren);

		// Render Players
		for (int player{ 0 }; player < d.PLAYERS; player++)
		{
			if (SDL_SetRenderDrawColor(ren, Game::colours[player][0], Game::colours[player][1], Game::colours[player][2], SDL_ALPHA_OPAQUE))
				SDLError("SetRenderDrawColor", win, ren);

			for (auto &point : pl[player].m_trail)
			{
				if (SDL_RenderFillRect(ren, &convertToRect(point, d.SCALE)))
					SDLError("RenderFillRext", win, ren);
			}
		}

		// Render Text and Overlays
		if (pause && !d.DEBUG)
		{
			if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_MOD))
				SDLError("SetRenderDrawBlendMode", win, ren);

			if (SDL_SetRenderDrawColor(ren, Game::pause[0], Game::pause[1], Game::pause[2], Game::pause[3]))
				SDLError("SetRenderDrawColor", win, ren);

			if (SDL_RenderFillRect(ren, NULL))
				SDLError("RenderFillRext", win, ren);

			pauseText.render(win, ren);

			if (winStr != " ")
				winText.render(win, ren);
		}
		else if (!d.DEBUG)
		{
			if (winStr == " ")
			{
				determineWinText(alive, aliveCount, winStr, roundWins, d.PLAYERS);
				if (winStr != " ")
					winText.change(win, ren, winStr, Text::TEXT_VICTORY);
			}
			winText.render(win, ren);

			if (aliveCount == 0)
				continueText.render(win, ren);
		}

		// Render Debug Grid
		if (d.DEBUG) 
		{
			if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
				SDLError("SetRenderDrawBlendMode", win, ren);

			if (SDL_SetRenderDrawColor(ren, Game::box[0], Game::box[1], Game::box[2], SDL_ALPHA_OPAQUE))
				SDLError("SetRenderDrawColor", win, ren);

			for (int x{ 0 }; x <= d.GRID.x; x++)
			{
				for (int y{ 0 }; y <= d.GRID.y; y++)
				{
					if (SDL_RenderDrawRect(ren, &convertToRect({ x, y }, d.SCALE)))
						SDLError("RenderFillRext", win, ren);
				}
			}
		}

		// Render Death Overlay
		for (int player{ 0 }; player < d.PLAYERS; player++)
			if ((alive[player] == false) && (countDown[player] > 0))
			{ 
				long double perc{ std::sin((countDown[player] * M_PI) / d.FLASH) };

				if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_ADD))
					SDLError("SetRenderDrawBlendMode", win, ren);


				std::cout << static_cast<int>(Game::colours[player][0] * perc) << " " <<
					static_cast<int>(Game::colours[player][1] * perc) << " " <<
					static_cast<int>(Game::colours[player][2] * perc) << "\n";

				if (SDL_SetRenderDrawColor(ren, static_cast<int>(Game::colours[player][0] * perc),
												static_cast<int>(Game::colours[player][1] * perc),
												static_cast<int>(Game::colours[player][2] * perc), 192))
					SDLError("SetRenderDrawColor", win, ren);

				if (SDL_RenderFillRect(ren, NULL))
					SDLError("RenderFillRext", win, ren);

				countDown[player]--;
			}

		// Update Screen
		SDL_RenderPresent(ren);
		SDL_Delay(d.SPEED);
		SDL_RenderClear(ren);

	}	
	return true;
}

bool gameEnd(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN, const std::vector<int> &roundWins)
{
	SDL_Event e{};

	std::vector<int> roundWinsCopy{ roundWins };
	std::vector<int> ranking(d.PLAYERS);

	for (int player{ 0 }; player < d.PLAYERS; player++)
		ranking[player] = player;

	for (int startI{ 0 }; startI < d.PLAYERS; startI++)
	{
		int largestI{ startI };
		for (int currentI{ startI + 1 }; currentI < d.PLAYERS; currentI++)
		{
			if (roundWinsCopy[currentI] > roundWinsCopy[largestI])
				largestI = currentI;
		}
		std::swap(roundWinsCopy[startI], roundWinsCopy[largestI]);
		std::swap(ranking[startI], ranking[largestI]);
	}
	
	Text title{ win, ren, "Final Scores", Text::TEXT_VICTORY };
	std::vector<Text> scoreboardL(d.PLAYERS, { win, ren, (" "), Text::TEXT_NONE });
	std::vector<Text> scoreboardR(d.PLAYERS, { win, ren, (" "), Text::TEXT_NONE });

#define TEXT_COLOUR { Game::text[0], Game::text[1], Game::text[2] }

	int count{ 0 };
	std::cout << "\n\nFINAL SCORES\n\n";
	for (int &position : ranking)
	{
		scoreboardL[position].change(win, ren, Game::colourNames[position], Text::getUsage(count), TEXT_COLOUR, 'L');
		scoreboardR[position].change(win, ren, std::to_string(roundWins[position]), Text::getUsage(count), TEXT_COLOUR, 'R');
		std::cout << (Game::colourNames[position] +  "\t\t" + std::to_string(roundWins[position])) << '\n';
		count++;
	}

#undef TEXT_COLOUR

	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			// If user closes the window
			if (e.type == SDL_QUIT)
				return false;

			// If user presses a key
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return false;

				if (e.key.keysym.sym == SDLK_SPACE)
					return true;
			}
		}

		// Render background
		if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
			SDLError("SetRenderDrawBlendMode", win, ren);

		if (SDL_SetRenderDrawColor(ren, Game::background[0], Game::background[1], Game::background[2], SDL_ALPHA_OPAQUE))
			SDLError("SetRenderDrawColor", win, ren);

		if (SDL_RenderFillRect(ren, NULL))
			SDLError("RenderFillRext", win, ren);

		title.render(win, ren);

		for (auto &rowL : scoreboardL)
			rowL.render(win, ren);
		for (auto &rowR : scoreboardR)
			rowR.render(win, ren);

		SDL_RenderPresent(ren);
		SDL_Delay(d.SPEED);
		SDL_RenderClear(ren);
	}
}

bool gameEndAlt(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN, const std::vector<int> &roundWins)
{
	SDL_Event e{};

	int winIndex{ 0 };
	for (int player{ 1 }; player < d.PLAYERS; player++)
		if (roundWins[player] > roundWins[winIndex])
			winIndex = player;

	Text title{ win, ren, (Game::colourNames[winIndex] + " wins"), Text::TEXT_CENTRE };

	// todo

	while (true)
	{
		while (SDL_PollEvent(&e))
		{
			// If user closes the window
			if (e.type == SDL_QUIT)
				return false;

			// If user presses a key
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return false;

				if (e.key.keysym.sym == SDLK_SPACE)
					return true;
			}
		}

		// Render background
		if (SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE))
			SDLError("SetRenderDrawBlendMode", win, ren);

		if (SDL_SetRenderDrawColor(ren, Game::colours[winIndex][0], Game::colours[winIndex][1], Game::colours[winIndex][2], SDL_ALPHA_OPAQUE))
			SDLError("SetRenderDrawColor", win, ren);

		if (SDL_RenderFillRect(ren, NULL))
			SDLError("RenderFillRext", win, ren);

		title.render(win, ren);

		SDL_RenderPresent(ren);
		SDL_Delay(d.SPEED);
		SDL_RenderClear(ren);
	}
}

bool game(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN, const std::string &PATH)
{
	std::cout << "NEW GAME:\n";
	if (d.FIRSTTO > 1) std::cout << "Playing to " << d.FIRSTTO << " wins\n";

	Text::setFont();
	Text::setScreenSize(SCREEN);

	std::vector<int> roundWins(d.PLAYERS);
	for (int player{ 0 }; player < d.PLAYERS; player++)
		roundWins[player] = 0;

	if ((d.GRID.x == d.GRID.y) && (!d.NOFRILLS))
	{
		if (!gameStart(win, ren, d, SCREEN)) return false;
	}

	do
	{
		if (!gameRound(win, ren, d, SCREEN, PATH, roundWins)) return false;

	} while (checkGameEnd(roundWins, d.FIRSTTO, d.PLAYERS));

	if (d.GRID.x == d.GRID.y && (!d.NOFRILLS))
	{
		if (!gameEnd(win, ren, d, SCREEN, roundWins)) return false;
	}
	else
	{
		if (!gameEndAlt(win, ren, d, SCREEN, roundWins)) return false;

	}

	return d.INFINITELOOP;
}
