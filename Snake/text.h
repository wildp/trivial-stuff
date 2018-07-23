#ifndef TEXT_H
#define TEXT_H

#include <string>

#define ESC "\x1b"
#define CSI "\x1b["

#define SPACER "          "
#define LINES_CLEARED 60

namespace Text
{
	const std::string title[]
	{
		R"(       _______..__   __.      ___       __  ___  _______    )",
		R"(      /       ||  \ |  |     /   \     |  |/  / |   ____|   )",
		R"(     |   (----`|   \|  |    /  ^  \    |  '  /  |  |__      )",
		R"(      \   \    |  . `  |   /  /_\  \   |    <   |   __|     )",
		R"(  .----)   |   |  |\   |  /  _____  \  |  .  \  |  |____    )",
		R"(  |_______/    |__| \__| /__/     \__\ |__|\__\ |_______|   )"
	};

	const std::string spaceToStart[]
	{
		"                  > Press space to start <                  ",
		"                    Press space to start                    "
	};

	const std::string instructionsTitle[]
	{
		R"(  ___            _                   _    _                 )",
		R"( |_ _| _ _   ___| |_  _ _  _  _  __ | |_ (_) ___  _ _   ___ )",
		R"(  | | | ' \ (_-<|  _|| '_|| || |/ _||  _|| |/ _ \| ' \ (_-< )",
		R"( |___||_||_|/__/ \__||_|   \_,_|\__| \__||_|\___/|_||_|/__/ )",
	};

	const std::string instructionsContent[]
	{
		"Use W, A, S, and D to change the direction of the snake's   ",
		"movement. Eat the food (red squares) to gain a point and    ",
		"increase the length of the snake by one square. The aim of  ",
		"the game is to get the highest score possible. If the head  ",
		"of the snake collides with another part of its body, or the ",
		"walls of the area, the game will be lost.                   ",
	};

	const std::string aboutTitle[]
	{
		R"(                  _    _                _                   )",
		R"(                 /_\  | |__  ___  _  _ | |_                 )",
		R"(                / _ \ | '_ \/ _ \| || ||  _|                )",
		R"(               /_/ \_\|_.__/\___/ \_,_| \__|                )"
	};

	const std::string aboutContent[]
	{
		"Command-line Snake                                          ",
		"Copyright 2018 Peter Wild                                   ",
		"Licensed under the Apache License, Version 2.0              ",
		"                                                            ",
		"The latest version of the source code can be found here:    ",
		"http://github.com/wildp/command-line-stuff/tree/master/Snake",
	};

	const std::string message[]
	{
		"               Press space to start the game.               ",
		"             Press q to quit or space to replay.            ",
	};

	const std::string gameOver[]
	{
		R"(   ____                            ___                      )",
		R"(  / ___|  __ _  _ __ ___    ___   / _ \ __   __ ___  _ __   )",
		R"( | |  _  / _` || '_ ` _ \  / _ \ | | | |\ \ / // _ \| '__|  )",
		R"( | |_| || (_| || | | | | ||  __/ | |_| | \ V /|  __/| |     )",
		R"(  \____| \__,_||_| |_| |_| \___|  \___/   \_/  \___||_|     )"
	};
}
#endif
