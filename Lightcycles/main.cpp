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
#include <array>
#include <bitset>
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <vector>
#include <wchar.h>
#include <Windows.h>

#include "text.h"

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

Coordinate getGridSize(const Coordinate &c)
{
	return { ((c.x - 2) / 2), (c.y - 2) };
}

Coordinate getWindowSize(const handleHolder &hand)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hand.out, &bufferInfo);
	return { bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1 };
}

handleHolder getHand()
{
	handleHolder hand;
	hand.out = GetStdHandle(STD_OUTPUT_HANDLE);
	hand.in = GetStdHandle(STD_INPUT_HANDLE);
	hand.hwnd = GetConsoleWindow();
	return hand;
}

std::array<short, 4> getKeyInput(const short &players, bool reset = false)
{
	constexpr int controls[4][4]
	{
		{ 'W', 'A', 'S', 'D' },
		{ VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT},
		{ VK_NUMPAD8, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6 },
		{ 'U', 'H', 'J', 'K'}
	};

	static std::array<short, 4> direction{ 3, 1, 2, 0 }; // N, W, S, E: 0, 1, 2, 3
	static std::array<std::bitset<4>, 4> keystates;

	if (reset == true)
		direction = { 3, 1, 2, 0 };

	for (int i{ 0 }; i < players; i++)
	{
		keystates[i][3] = (GetKeyState(controls[i][0]) < 0);
		keystates[i][2] = (GetKeyState(controls[i][1]) < 0);
		keystates[i][1] = (GetKeyState(controls[i][2]) < 0);
		keystates[i][0] = (GetKeyState(controls[i][3]) < 0);

		switch (keystates[i].to_ulong())
		{
		case 0b1000: direction[i] = (direction[i] != 2) ? 0 : direction[i]; break;
		case 0b0100: direction[i] = (direction[i] != 3) ? 1 : direction[i]; break;
		case 0b0010: direction[i] = (direction[i] != 0) ? 2 : direction[i]; break;
		case 0b0001: direction[i] = (direction[i] != 1) ? 3 : direction[i]; break;

		case 0b1100: direction[i] = (direction[i] != 2 && direction[i] != 3) ? ((direction[i] == 0) ? 1 : 0) : direction[i]; break;
		case 0b1001: direction[i] = (direction[i] != 2 && direction[i] != 1) ? ((direction[i] == 0) ? 3 : 0) : direction[i]; break;
		case 0b0110: direction[i] = (direction[i] != 0 && direction[i] != 3) ? ((direction[i] == 2) ? 1 : 2) : direction[i]; break;
		case 0b0011: direction[i] = (direction[i] != 0 && direction[i] != 1) ? ((direction[i] == 2) ? 3 : 2) : direction[i]; break;

		default: break; // no change to direction
		}
	}
	// std::cout << keystates << ' ' << direction << '\n';
	return direction;
}

inline int getHalf(const int &full) // possibly incorrect
{
	return ((full % 2) == 0) ? (full / 2) : ((full - 1)/2);
}

std::array<Coordinate, 4> getStartPosition(const Coordinate &gridSize) // possibly incorrect
{
	std::array<Coordinate, 4> startPosition
	{
		Coordinate{ 0, getHalf(gridSize.y) },
		Coordinate{ gridSize.x, getHalf(gridSize.y) },
		Coordinate{ getHalf(gridSize.x), 0  },
		Coordinate{ getHalf(gridSize.x), gridSize.y }
	};
	return startPosition;
}

class Player
{
private:
	bool playerAlive;
	Coordinate playerHead;
	std::vector<Coordinate> playerTrail;

public:
	Player():
		playerHead{ -1, -1 },
		playerTrail{ playerHead },
		playerAlive{ false }
	{
		return;
	}

	Player(const bool isPlayer, const Coordinate start) :
		playerHead{ start },
		playerTrail{ playerHead },
		playerAlive{ isPlayer }
	{
		return;
	}

	void move(const short &direction)
	{
		if (playerAlive)
		{
			switch (direction)
			{
			case 0: --playerHead.y; break;
			case 1: --playerHead.x; break;
			case 2: ++playerHead.y; break;
			case 3: ++playerHead.x; break;
			}
			playerTrail.push_back(playerHead);
		}
	}

	void collide(const Coordinate &gridSize, std::array<Player, 4> &others, const int &players, const int &blacklist)
	{
		if (playerHead.x <= 0 || playerHead.y <= 0 || playerHead.x >= gridSize.x || playerHead.y >= gridSize.y)
			playerAlive = false;

		for (int i{ 0 }; i < players; i++)
		{
			std::vector<Coordinate> &trail{ others[i].playerTrail };

			for (unsigned int j{ 0 }; j < ((i == blacklist) ? (trail.size() - 1) : (trail.size())); j++)
				if ((playerHead.x == trail[j].x) && (playerHead.y == trail[j].y))
					playerAlive = false;
		}
	}

	bool alive()
	{
		return playerAlive;
	}

	friend void renderGame(const handleHolder &hand, const Coordinate &gridSize, const int &players, std::array<Player, 4> &playerArray, const int &aliveCount, const int &winner, bool reset);
};

void hideCursor(const handleHolder &hand, bool undo = false)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hand.out, &cursorInfo);
	static const CONSOLE_CURSOR_INFO cursorInfoOriginal{ cursorInfo };

	if (undo == false)
	{
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(hand.out, &cursorInfo);
	}
	else if (undo == true)
	{
		SetConsoleCursorInfo(hand.out, &cursorInfoOriginal);
	}
	return;
}

void changeCursorY(const handleHolder &hand, int position)
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hand.out, &bufferInfo);
	bufferInfo.dwCursorPosition.Y += position;
	SetConsoleCursorPosition(hand.out, bufferInfo.dwCursorPosition);
	return;
}

void cls(const handleHolder &hand, short maximum, short start = 0, short repetitions = 1)
{
	static const char write[121]{ "                                                                                                                      " };

	for (short i{ start }; i <= (maximum + start); ++i)
	{
		SetConsoleCursorPosition(hand.out, { 0, i });
		unsigned long charsWritten;

		for (int i{0}; i < repetitions; i++)
			WriteConsole(hand.out, &write, 121, &charsWritten, NULL);
	}
	SetConsoleCursorPosition(hand.out, { 0, start });
	return;
}

short pow10(short count)
{
	if (count > 0)
		return (10 * pow10(count - 1));
	else
		return 1;
}

void stripZero(std::vector<int> &vec)
{
	if (vec[vec.size() - 1] == 0 && vec.size() > 1)
	{
		vec.pop_back();
		return stripZero(vec);
	}
	else
		return;
}

inline void inputPrompt()
{
	std::cout << "\n" MINI_SPACER << Text::speed[0];
	std::cout << "\n" MINI_SPACER "> ";
	return;
}

int menu(const handleHolder &hand, short &gameSpeed)
{
	cls(hand, LINES_CLEARED * 2);
	hideCursor(hand);
	SetConsoleCursorPosition(hand.out, { 0, 0 });

	for (auto line : Text::title)
		std::cout << MINI_SPACER << line << '\n';

	SetConsoleCursorPosition(hand.out, { 0, 3 });

	for (int i{ 0 }; i < 2; i++)
		std::cout << "\n" MINI_SPACER << Text::menu[i];

	static int players{ 2 };

	while (true)
	{
		hideCursor(hand);

		if (GetKeyState('2') < 0)
		{
			players = 2;
			cls(hand, 10, 3);
			std::cout << "\n" MINI_SPACER << Text::players[0] << '\n';
			std::cout << MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState('4') < 0)
		{
			players = 4;
			cls(hand, 10, 3);
			std::cout << "\n" MINI_SPACER << Text::players[1] << '\n';
			std::cout << MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState('H') < 0)
		{
			cls(hand, 10, 3);
			for (auto line : Text::help)
				std::cout << "\n" MINI_SPACER << line;
			std::cout << "\n\n" MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState('S') < 0)
		{
			cls(hand, 10, 3);
			FlushConsoleInputBuffer(hand.in);

			std::vector<int> input{ 0 };
			bool loop{ true };

			inputPrompt();

			while (loop == true)
			{
				hideCursor(hand);

				for (int i{ 0 }; i < 10; i++)
				{
					if (GetKeyState(static_cast<char>(i + 48)) < 0)
					{
						std::cout << static_cast<char>(i + 48);
						input.insert(input.cbegin(), i);

						while (GetKeyState(static_cast<char>(i + 48)) < 0)
							Sleep(1);
					}
				}

				if (GetKeyState(VK_BACK) < 0)
				{
					cls(hand, 10, 3);
					input.clear();
					input.push_back(0);
					inputPrompt();

					while (GetKeyState(VK_BACK) < 0)
						Sleep(1);
				}

				if (GetKeyState(VK_RETURN) < 0)
				{
					stripZero(input);

					if (input.size() > 4)
					{
						gameSpeed = 0;
						cls(hand, 10, 3);
						input.clear();
						input.push_back(0);
						std::cout << "\n" MINI_SPACER "The entered number is too large";
						inputPrompt();
					}
					else
					{
						gameSpeed = 0;
						for (int i{ 0 }; i < static_cast<int>(input.size()); i++)
						{
							gameSpeed += input[i] * pow10(static_cast<short>(i));
						}
						//std::cout << "\n\nThe number you entered was " << gameSpeed;
					}

					if (gameSpeed > 0 && gameSpeed < 10000)
						break;
					else
					{
						cls(hand, 10, 3);
						input.clear();
						input.push_back(0);
						std::cout << "\n" MINI_SPACER "The entered number is not valid";
						inputPrompt();
					}

					while (GetKeyState(VK_RETURN) < 0)
						Sleep(1);
				}
			}

			FlushConsoleInputBuffer(hand.in);
			std::cout << "\n" MINI_SPACER << Text::speed[1];
			std::cout << "\n\n" MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState('C') < 0)
		{
			cls(hand, 10, 3);
			for (auto line : Text::controls)
				std::cout << "\n" MINI_SPACER << line;
			std::cout << "\n\n" MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState('A') < 0)
		{
			cls(hand, 10, 3);
			for (auto line : Text::about)
				std::cout << "\n" MINI_SPACER << line;
			std::cout << "\n\n" MINI_SPACER << Text::menu[1];
		}
		if (GetKeyState(VK_SPACE) < 0)
		{
			return players;
		}
		if (GetKeyState(VK_ESCAPE) < 0)
		{
			exit(0);
		}
	}
}

int replay(const handleHolder &hand, int &winner)
{
	for (auto line : Text::over)
		std::cout << MINI_SPACER << line << '\n';

	std::cout << MINI_SPACER << Text::message[winner];
	std::cout << "\n" MINI_SPACER << Text::menu[2];

	while (true)
	{
		hideCursor(hand);

		if (GetKeyState('M') < 0)
			return 1;
		if (GetKeyState('Q') < 0)
			return -1;	
		if (GetKeyState(VK_SPACE) < 0)
			return 0;
		if (GetKeyState(VK_ESCAPE) < 0)
			return -1;
	}
}

inline void renderGameSwitch(const handleHolder &hand, const char &symbol)
{
	switch (symbol)
	{
	case '.': // background: black
		SetConsoleTextAttribute(hand.out, 0);
		break;
	case 'f': // food (snake): bright red
		SetConsoleTextAttribute(hand.out, 204);
		break;
	case 's': // snake (snake): bright white
		SetConsoleTextAttribute(hand.out, 255);
		break;
	case '1': // player 1: bright green
		SetConsoleTextAttribute(hand.out, 170);
		break;
	case '2': // player 2: bright purple
		SetConsoleTextAttribute(hand.out, 221);
		break;
	case '3': // player 3: bright blue
		SetConsoleTextAttribute(hand.out, 187);
		break;
	case '4': // player 4: bright yellow
		SetConsoleTextAttribute(hand.out, 238);
		break;
	case 'w': // wall (snake): bright black
		SetConsoleTextAttribute(hand.out, 136);
		break;
	}
	return;
}

inline void renderGameHelper(const handleHolder &hand, const Coordinate &gridSize, const std::vector<std::vector<char>> &map, const short &j)
{
	SetConsoleCursorPosition(hand.out, { 0, j });

	for (int i{ 0 }; i <= gridSize.x; ++i)
	{
		renderGameSwitch(hand, map[i][j]);

		DWORD charsWritten;
		char write[2]{ map[i][j] , map[i][j] };
		WriteConsole(hand.out, &write, 2, &charsWritten, NULL);
	}
	return;
}

inline void renderGameInsert(const handleHolder &hand, const char &symbol, const Coordinate &coord)
{
	SetConsoleCursorPosition(hand.out, { static_cast<short>(coord.x) * 2, static_cast<short>(coord.y) });
	renderGameSwitch(hand, symbol);

	DWORD charsWritten;
	char write[2]{ symbol , symbol };
	WriteConsole(hand.out, &write, 2, &charsWritten, NULL);
}

void renderGame(const handleHolder &hand, const Coordinate &gridSize, const int &players, std::array<Player, 4> &playerArray, const int &aliveCount, const int &winner, bool reset = false)
{
	static std::vector<std::vector<char>> map(gridSize.x + 1, std::vector<char>(gridSize.y + 1, '.'));

	static bool mapInit{ false };
	static bool mapRendered{ false };

	if (reset == true)
	{
		map.assign(gridSize.x + 1, std::vector<char>(gridSize.y + 1, '.'));
		mapInit = false;
		mapRendered = false;
	}

	if (mapInit == false)
	{
		mapInit = true;

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
	

	for (int i{ 0 }; i < players; i++)
	{
		if (playerArray[i].playerHead.x > 0 && playerArray[i].playerHead.y > 0 && playerArray[i].playerHead.x < gridSize.x && playerArray[i].playerHead.y < gridSize.y)
			if (map[playerArray[i].playerHead.x][playerArray[i].playerHead.y] == '.')
			{
				map[playerArray[i].playerHead.x][playerArray[i].playerHead.y] = static_cast<char>(i + 49);
				if (mapRendered == true)
					renderGameInsert(hand, static_cast<char>(i + 49), playerArray[i].playerHead);
			}
	}
	
	if (mapRendered == false)
	{
		for (short j{ 0 }; j <= gridSize.y; j += 2)
			renderGameHelper(hand, gridSize, map, j);

		for (short j{ 1 }; j <= gridSize.y; j += 2)
			renderGameHelper(hand, gridSize, map, j);
		
		mapRendered = true;
	}

	SetConsoleTextAttribute(hand.out, 7);

	if (aliveCount == 1 && winner != 0)
	{
		SetConsoleCursorPosition(hand.out, { 0, static_cast<SHORT>(gridSize.y + 1) });
		DWORD charsWritten;
		std::string write{ Text::message[winner] };
		WriteConsole(hand.out, write.c_str(), static_cast<unsigned int>(write.length()), &charsWritten, NULL);
	}
}

int game(const handleHolder &hand, const Coordinate &gridSize, const short &gameSpeed, const short &players)
{
	cls(hand, gridSize.y);

	bool inGame{ true };
	bool reset{ true };
	int aliveCount{ 0 };
	int winner{ 0 };

	std::array<short, 4> direction;
	std::array<Coordinate, 4> startPosition{ getStartPosition(gridSize) };

	std::array<Player, 4> playerArray{};

	for (int i{ 0 }; i < 4; i++)
		playerArray[i] = Player(((i < players) ? true : false), ((i < players) ? startPosition[i] : Coordinate{ -1, -1 }));

	do
	{
		direction = getKeyInput(players, reset);

		for (int i{ 0 }; i < players; i++)
			playerArray[i].move(direction[i]);
		
		for (int i{ 0 }; i < players; i++)
			playerArray[i].collide(gridSize, playerArray, players, i);

		aliveCount = 0;
		for (int i{ 0 }; i < players; i++)
			aliveCount += playerArray[i].alive();

		if (aliveCount == 1 && winner == 0)
			for (int i{ 0 }; i < players; i++)
				if (playerArray[i].alive() == true)
					winner = i + 1;				

		renderGame(hand, gridSize, players, playerArray, aliveCount, winner, reset);

		reset = false;

		Sleep(gameSpeed);

		if (GetKeyState(VK_ESCAPE) < 0)
			exit(0);

	} while (aliveCount != 0);

	cls(hand, gridSize.y + 2, 0, 1 + (gridSize.x / 60));
	return winner;
}

void setup(const handleHolder &hand, int mode , COORD size = { 0, 0 })
{
	// Note for mode:
	//     Left bit:  Use to change and restore window and buffer size
	//     Right bit: Determines if the cursor is visible

	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
	GetConsoleScreenBufferInfo(hand.out, &bufferInfo);
	static const CONSOLE_SCREEN_BUFFER_INFO bufferInfoOriginal{ bufferInfo };

	static const COORD windowSize = { bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1 };
	//std::cout << windowSize.X << " " << windowSize.Y << '\n';

	if (mode == 0b00 || mode == 0b01)
	{
		SMALL_RECT info = { 0, 0, size.X - 1, size.Y - 1 };
		SetConsoleWindowInfo(hand.out, TRUE, &info);
		SetConsoleScreenBufferSize(hand.out, size);
		SetConsoleWindowInfo(hand.out, TRUE, &info);
	}
	else if (mode == 0b10 || mode == 0b11)
	{
		SMALL_RECT info = { 0, 0, windowSize.X - 1, windowSize.Y - 1 };
		SetConsoleWindowInfo(hand.out, TRUE, &info);
		SetConsoleScreenBufferSize(hand.out, { windowSize.X, 9001 });
		SetConsoleWindowInfo(hand.out, TRUE, &info);
	}

	if (mode == 0b00 || mode == 0b10)
	{
		hideCursor(hand);
	}
	else if (mode == 0b01 || mode == 0b01)
	{
		hideCursor(hand, true);
	}

	return cls(hand, LINES_CLEARED);
}

void setup(const handleHolder &hand, int mode, Coordinate size)
{
	return setup(hand, mode, COORD{ static_cast<short>(size.x), static_cast<short>(size.y) });
}

int main()
{
	short gameSpeed{ 40 }; 
	
	handleHolder hand = getHand();
	SetConsoleTitle("Lightcycles");
	setup(hand, 0b01);

	int score{ 0 };
	int winner{ 0 };
	int action{ 0 };
	int players{ 0 };
	Coordinate targetSize{};

restart:

	players = menu(hand, gameSpeed);

	do
	{
		targetSize = getWindowSize(hand);
		setup(hand, 0b00, targetSize);
		winner = game(hand, getGridSize(targetSize), gameSpeed, players);
		action = replay(hand, winner);


	} while (action == 0);

	if (action == 1)
		goto restart;

	setup(hand, 0b11);
	FlushConsoleInputBuffer(hand.in);
	return 0;
}
