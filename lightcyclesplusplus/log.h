// logerror.h
// stores a foreward declaration for logSDLError()

#ifndef LOGERROR_H
#define LOGERROR_H

#include <SDL.h>

#include "struct.h"

void logSDLError(const std::string &msg);

void SDLError(const std::string &msg, SDL_Window *win, SDL_Renderer *ren);

void logSettings(Data &d);

#endif // !LOGERROR_H

