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


#ifndef TEXT_H
#define TEXT_H

#include <string>

#define ESC "\x1b"
#define CSI "\x1b["

#define SPACER "         "
#define MINI_SPACER "  "
#define LINES_CLEARED 60


namespace Text
{
	const std::string title[]
	{
		R"(        __       ___  __       __        ___  __  )",
		R"(|    | / _` |__|  |  /  ` \ / /  ` |    |__  /__` )",
		R"(|___ | \__> |  |  |  \__,  |  \__, |___ |___ .__/ )",
		R"(                                                  )",
	};

	const std::string over[]
	{
		R"( __          ___    __      ___ __ )",
		R"(/ _` /\ |\/||__    /  \\  /|__ |__))",
		R"(\__>/--\|  ||___   \__/ \/ |___|  \)",
		R"(                                   )",
	};
                                                 
	const std::string menu[]
	{
		"Press h to view help, or 2 or 4 to set number of players.             ",
		"Press space to start.                                                 ",
		"Press q to quit, m to return to menu, or space to replay.             ",
	};

	const std::string players[]
	{
		"Number of players has been set to 2.                                  ",
		"Number of players has been set to 4.                                  ",
	};

	const std::string help[]
	{
		"General Help:                                                         ",
		"    Press  h  to view this menu.                                      ",
		"    Press  c  to view in-game controls.                               ",
		"    Press  a  to view information.                                    ",
		"    Press  s  to change game speed.                                   ",
		"    Press  2  or  4  to set number of players (Default 2).            ",
		"                                                                      ",
		"Resize the command prompt window while in this menu to change the size",
		"of the game area. Resizing the command prompt window while playing the",
		"game will result in visual artifacts and general graphical problems.  ",
	};

	const std::string controls[]
	{
		"The controls are as follows:                                          ",
		"    Player 1 (bright green)  use  W A S D                             ",
		"    Player 2 (bright purple) use  ARROW KEYS                          ",
		"    Player 3 (bright cyan)   use  NUMPAD 8 4 5 6                      ",
		"    Player 4 (bright yellow) use  U H J K                             ",
	};

	const std::string speed[]
	{
		"Enter a new game speed below (default 40)                             ",
		"Game speed has been changed.                                          ",
	};

	const std::string about[]
	{
		"Command-line Lightcycles                                              ",
		"Copyright 2018 Peter Wild                                             ",
		"Licensed under the Apache License, Version 2.0                        ",
		"                                                                      ",
		"The latest version of the source code can be found here:              ",
		"http://github.com/wildp/command-line-stuff/tree/master/Lightcycles    ",
	};

	const std::string message[]
	{
		"No one has won the game.      ",
		"Player 1 has won the game.    ",
		"Player 2 has won the game.    ",
		"Player 3 has won the game.    ",
		"Player 4 has won the game.    ",
	};
}
#endif
