// game.h
// stores a forward declaration for game() in game.cpp

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <string>
#include <vector>

#include "struct.h"

bool game(SDL_Window *win, SDL_Renderer *ren, const Data &d, const SDL_Point &SCREEN, const std::string &PATH);

#endif // !GAME_H
