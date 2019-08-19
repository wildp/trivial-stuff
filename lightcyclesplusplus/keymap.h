// keymap.h
// stores the keymap for each player

#ifndef KEYMAP_H
#define KEYMAP_H

#include <SDL.h>

namespace Game
{
	constexpr SDL_Scancode keymap[4][4]	//[x][y]
	{	// format: up, left, down, right (e.g. WASD)
		{SDL_SCANCODE_W,	SDL_SCANCODE_A,		SDL_SCANCODE_S,		SDL_SCANCODE_D		},
		{SDL_SCANCODE_UP,	SDL_SCANCODE_LEFT,	SDL_SCANCODE_DOWN,	SDL_SCANCODE_RIGHT	},
		{SDL_SCANCODE_KP_8,	SDL_SCANCODE_KP_4,	SDL_SCANCODE_KP_5,	SDL_SCANCODE_KP_6	},
		{SDL_SCANCODE_U,	SDL_SCANCODE_H,		SDL_SCANCODE_J,		SDL_SCANCODE_K		}
	};
}

#endif // !KEYMAP_H

