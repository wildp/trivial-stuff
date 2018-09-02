// Copyright 2018 Peter Wild
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#define WIN32_LEAN_AND_MEAN
#include <bitset>
#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <wchar.h>
#include <Windows.h>
	
#include "text.h"

// DESIRED CONSOLE SCREEN SIZE = { 84, 30 }

struct handleHolder
{
	HANDLE out;
	HANDLE in;
	HWND hwnd;
};

struct Coordinate
{
	int x;
	int y;
};

bool operator==(const Coordinate &c1, const Coordinate &c2)
{
	if (c1.x == c2.x && c1.y == c2.y)
		return true;
	else
		return false;
}

bool operator!=(const Coordinate &c1, const Coordinate &c2)
{
	return !(c1 == c2);
}

int getKeyInput(bool reset = false)
{
	static int direction{ 3 }; // N, W, S, E: 0, 1, 2, 3
	static std::bitset<4> keystates;

	if (reset == true)
		direction = 3;

	keystates[3] = (GetKeyState('W') < 0);
	keystates[2] = (GetKeyState('A') < 0);
	keystates[1] = (GetKeyState('S') < 0);
	keystates[0] = (GetKeyState('D') < 0);

	switch (keystates.to_ulong())
	{
	case 0b1000: direction = (direction != 2) ? 0 : direction; break;
	case 0b0100: direction = (direction != 3) ? 1 : direction; break;
	case 0b0010: direction = (direction != 0) ? 2 : direction; break;
	case 0b0001: direction = (direction != 1) ? 3 : direction; break;

	case 0b1100: direction = (direction != 2 && direction != 3) ? ((direction == 0) ? 1 : 0) : direction; break;
	case 0b1001: direction = (direction != 2 && direction != 1) ? ((direction == 0) ? 3 : 0) : direction; break;
	case 0b0110: direction = (direction != 0 && direction != 3) ? ((direction == 2) ? 1 : 2) : direction; break;
	case 0b0011: direction = (direction != 0 && direction != 1) ? ((direction == 2) ? 3 : 2) : direction; break;

	default: break; // no change to direction
	}

	// std::cout << keystates << ' ' << direction << '\n';
	return direction;
}

void changeCursorY(const handleHolder &hand, int position)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hand.out, &bufferInfo);
	bufferInfo.dwCursorPosition.Y += position;
	SetConsoleCursorPosition(hand.out, bufferInfo.dwCursorPosition);
	return;
}

void cls(const handleHolder &hand, int maximum)
{
	for (short i{ 0 }; i <= maximum; ++i)
	{
		SetConsoleCursorPosition(hand.out, { 0, i });
		DWORD charsWritten;
		char write[121]{ "                                                                                                                      " };
		WriteConsole(hand.out, &write, 121, &charsWritten, NULL);
	}
	SetConsoleCursorPosition(hand.out, { 0, 0 });
	return;
}

void mainMenu(const handleHolder &hand)
{
	std::cout << "\n\n\n\n";

	for (auto line : Text::title)
		std::cout << SPACER << line << '\n';

	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n"; 

	while (!(GetKeyState(' ') < 0))
	{
		static unsigned int count{ 0 };

		changeCursorY(hand, -1);

		if (count % 20 < 5)
			std::cout << SPACER << Text::spaceToStart[0] << '\n';
		else
			std::cout << SPACER << Text::spaceToStart[1] << '\n';

		Sleep(50);
		++count;
	}
	std::cout << "\n\n";
	return cls(hand, LINES_CLEARED);
}

void helpMenu(const handleHolder &hand)
{
	std::cout << "\n\n";

	for (auto line : Text::instructionsTitle)
		std::cout << SPACER << line << '\n';

	std::cout << "\n";

	for (auto line : Text::instructionsContent)
		std::cout << SPACER << line << '\n';

	std::cout << "\n\n";

	for (auto line : Text::aboutTitle)
		std::cout << SPACER << line << '\n';

	std::cout << "\n";

	for (auto line : Text::aboutContent)
		std::cout << SPACER << line << '\n';

	std::cout << "\n\n\n\n\n";

	Sleep(1000);

	changeCursorY(hand, -2);
	std::cout << SPACER << Text::message[0] << "\n\n";

	while (!(GetKeyState(' ') < 0))
		Sleep(50);
	return cls(hand, LINES_CLEARED);
}

bool replayMenu(const handleHolder &hand, int &score)
{
	std::cout << "\n\n\n\n\n\n\n\n";

	for (auto line : Text::gameOver)
		std::cout << SPACER << line << '\n';

	std::cout << "\n\n\n\n\n\n" SPACER SPACER SPACER "Final score: " << score << " points \n";
	std::cout << "\n\n\n\n" SPACER << Text::message[1] << "\n\n";

	while (true)
	{
		if (GetKeyState('Q') < 0)
			return false;
		if (GetKeyState(' ') < 0)
			return true;

		Sleep(50);
	}
}

inline void renderGameHelper(const handleHolder &hand, const Coordinate &gridSize, const std::vector<std::vector<char>> &map, const short &j)
{
	SetConsoleCursorPosition(hand.out, { 0, j });

	for (int i{ 0 }; i <= gridSize.x; ++i)
	{
		switch (map[i][j])
		{
		case '.':
			SetConsoleTextAttribute(hand.out, 0);
			break;
		case 'f':
			SetConsoleTextAttribute(hand.out, 204);
			break;
		case 's':
			SetConsoleTextAttribute(hand.out, 255);
			break;
		case 'w':
			SetConsoleTextAttribute(hand.out, 136);
			break;
		}

		DWORD charsWritten;
		char write[2]{ map[i][j] , map[i][j] };
		WriteConsole(hand.out, &write, 2, &charsWritten, NULL);
	}
	return;
}

void renderGame(const handleHolder &hand, const Coordinate &gridSize, const Coordinate &snakeFood, const std::vector<Coordinate> &snakeTrail, bool reset = false)
{
	static std::vector<std::vector<char>> map(gridSize.x + 1, std::vector<char>(gridSize.y + 1, '.'));

	static Coordinate food{ snakeFood };
	static Coordinate snakeEnd{ 0, 0 };
	static auto snakeLength{ snakeTrail.size() };

	static bool mapInit{ false };
	static bool foodRendered{ false };

	if (reset == true)
	{
		map.assign(gridSize.x + 1, std::vector<char>(gridSize.y + 1, '.'));
		mapInit = false;

		snakeEnd = { 0,0 };
		snakeLength = snakeTrail.size();

		food = snakeFood;
		foodRendered = false;
	}

	if (mapInit == false)
	{
		mapInit = true;
		map[snakeTrail[0].x][snakeTrail[0].y] = 's';
		map[snakeTrail[1].x][snakeTrail[1].y] = 's';
		map[snakeTrail[2].x][snakeTrail[2].y] = 's';

		for (int j{ 0 }; j <= gridSize.y; ++j)
		{
			map[0][j] = 'w';
			map[gridSize.x][j] = 'w';
		}
		for (int i{ 0 }; i <= gridSize.x; ++i)
		{
			map[i][0] = 'w';
			map[i][gridSize.y] = 'w';
		}
	}

	if (snakeFood != food)
	{
		foodRendered = false;
		map[food.x][food.y] = '.';
		food = snakeFood;
	}
	if (foodRendered == false && map[food.x][food.y] == '.')
	{
		map[food.x][food.y] = 'f';
		foodRendered = true;
	}

	map[snakeTrail[0].x][snakeTrail[0].y] = 's';

	if (snakeLength < snakeTrail.size())
	{
		snakeLength = snakeTrail.size();
		snakeEnd = { 0,0 };
	}
	else
	{
		if (map[snakeEnd.x][snakeEnd.y] == 's')
			map[snakeEnd.x][snakeEnd.y] = '.';
	}
	snakeEnd = snakeTrail[snakeLength - 1];

	for (short j{ 0 }; j <= gridSize.y; j += 2)
	{
		renderGameHelper(hand, gridSize, map, j);
	}

	for (short j{ 1 }; j <= gridSize.y; j += 2)
	{
		renderGameHelper(hand, gridSize, map, j);
	}
	SetConsoleTextAttribute(hand.out, 7);

	SetConsoleCursorPosition(hand.out, { 0, static_cast<SHORT>(gridSize.y + 1) });
	DWORD charsWritten;
	std::string write{ "Current Score: " };
	write.append(std::to_string(snakeLength - 3));
	WriteConsole(hand.out, write.c_str(), static_cast<unsigned int>(write.length()), &charsWritten, NULL);
}

int game(const handleHolder &hand, Coordinate gridSize, const short &gameSpeed)
{
	cls(hand, LINES_CLEARED);

	bool inGame{ true };

	std::mt19937 generator(GetTickCount());
	generator();
	std::uniform_int_distribution<int> xDistribution(1, gridSize.x - 1);
	std::uniform_int_distribution<int> yDistribution(1, gridSize.y - 1);

	short direction;
	Coordinate snakeHead{ 10, 15 };
	Coordinate snakeFood{ xDistribution(generator), yDistribution(generator) };
	std::vector<Coordinate> snakeTrail{ snakeHead, {snakeHead.x - 1, snakeHead.y},{ snakeHead.x - 2, snakeHead.y } };
	bool reset = true;

	do
	{
		direction = getKeyInput(reset);

		switch(direction)
		{
		case 0: --snakeHead.y; break;
		case 1: --snakeHead.x; break;
		case 2: ++snakeHead.y; break;
		case 3: ++snakeHead.x; break;
		}

		if (snakeHead.x <= 0 || snakeHead.y <= 0 || snakeHead.x >= gridSize.x || snakeHead.y >= gridSize.y)
		{
			inGame = false;
			goto stop;
		}

		for (unsigned int i{ 1 }; i < snakeTrail.size() - 1; ++i)
		{
			if (snakeHead == snakeTrail[i])
			{
				inGame = false;
				goto stop;
			}
		}

		snakeTrail.insert(snakeTrail.cbegin(), snakeHead);

		if (snakeHead == snakeFood)
		{
			bool fail{ true };

			while (fail)
			{
				snakeFood = { xDistribution(generator), yDistribution(generator) };
				fail = false;
				for (unsigned int i{ 0 }; i < snakeTrail.size() - 1; ++i)
				{
					if (snakeFood == snakeTrail[i])
					{
						fail = true;
					}
				}
			}
		}
		else
			snakeTrail.pop_back();

		renderGame(hand, gridSize, snakeFood, snakeTrail, reset);
		reset = false;
	stop:
		Sleep(gameSpeed);
		
	} while (inGame);

	cls(hand, LINES_CLEARED);
	return (static_cast<int>(snakeTrail.size() - 3));
}

void setup(const handleHolder &hand, bool undo = false, COORD size = { 84, 30 })
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hand.out, &cursorInfo);
	static const CONSOLE_CURSOR_INFO cursorInfoOriginal{ cursorInfo };

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hand.out, &bufferInfo);
	static const CONSOLE_SCREEN_BUFFER_INFO bufferInfoOriginal{ bufferInfo };

	static COORD windowSize = { bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1 };
	std::cout << windowSize.X << " " << windowSize.Y << '\n';

	if (undo == false)
	{
		SetConsoleTitle("Snake");
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hand.out, &cursorInfo);

		SMALL_RECT info = { 0, 0, size.X - 1, size.Y - 1 };
		std::cout << SetConsoleWindowInfo(hand.out, TRUE, &info);
		std::cout << SetConsoleScreenBufferSize(hand.out, size);
		std::cout << SetConsoleWindowInfo(hand.out, TRUE, &info);
	}
	else if (undo == true)
	{
		SetConsoleCursorInfo(hand.out, &cursorInfoOriginal);

		SMALL_RECT info = { 0, 0, windowSize.X - 1, windowSize.Y - 1 };
		std::cout << SetConsoleWindowInfo(hand.out, TRUE, &info);
		std::cout << SetConsoleScreenBufferSize(hand.out, {windowSize.X, 9001});
		std::cout << SetConsoleWindowInfo(hand.out, TRUE, &info);
		// + delete input while in game
	}

	//Sleep(1000);
	return cls(hand, LINES_CLEARED);
}

int main()
{
	short gameSpeed{ 5 };

	handleHolder hand;
	hand.out = GetStdHandle(STD_OUTPUT_HANDLE);
	hand.in = GetStdHandle(STD_INPUT_HANDLE);
	hand.hwnd = GetConsoleWindow();

	setup(hand);
mainMenu(hand);
	helpMenu(hand);

	int score{ 0 };
	int scoreBest{ 0 };
	do
	{
		score = game(hand, { 41, 28 }, gameSpeed);
		if (score > scoreBest)
			scoreBest = score;

	} while (replayMenu(hand, score));

	setup(hand, true);
	std::cout << "Best score this session: " << scoreBest << " points.";
	return 0;
}
