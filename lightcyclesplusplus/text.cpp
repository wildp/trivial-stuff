// text.cpp
// contains code for the Text class

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

#include "colours.h"
#include "log.h"

#include "text.h"

// defaults in case setFont and setScreenSize are not called
std::string Text::s_fontPath	= "C:\\Windows\\Fonts\\arialbd.ttf";
SDL_Point Text::s_screenSize	= { 0, 0 };

void Text::setFont(std::string PATH) //static
{
	if (PATH == " ") PATH = "C:\\Windows\\Fonts\\arialbd.ttf";
	else s_fontPath = PATH + "font.ttf";
}

void Text::setScreenSize(const SDL_Point &SCREEN) //static
{
	s_screenSize = SCREEN;
}

Text::Usage Text::getUsage(const int row) //static
{
	switch (row)
	{
	case 0:  return Text::TEXT_TABLE_ROW_0;
	case 1:  return Text::TEXT_TABLE_ROW_1;
	case 2:  return Text::TEXT_TABLE_ROW_2;
	case 3:  return Text::TEXT_TABLE_ROW_3;
	default: std::cout << "\tWarning\n"; return Text::TEXT_GENERAL;
	}
}

const int Text::getFontSize(std::string message, Usage type)
{
	int len{ static_cast<int>(message.length()) };

	switch (type)
	{
	case TEXT_VICTORY:
		return static_cast<int>((s_screenSize.x / 11) * 1.6l);
	case TEXT_PAUSE:
		return static_cast<int>((s_screenSize.x /  6) * 1.2l);
	case TEXT_CONTINUE:
		return static_cast<int>((s_screenSize.x / 24) * 1.8l);
	case TEXT_START:
		return static_cast<int>((s_screenSize.x / 22) * 1.2l);
	case TEXT_WINS:
		return static_cast<int>((s_screenSize.x / 28) * 1.2l);
	case TEXT_CENTRE:
		return static_cast<int>((s_screenSize.x / 11) * 1.6l);

	case TEXT_TABLE_ROW_0:
		return static_cast<int>((s_screenSize.x / 14));
	case TEXT_TABLE_ROW_1:
		return static_cast<int>((s_screenSize.x / 14));
	case TEXT_TABLE_ROW_2:
		return static_cast<int>((s_screenSize.x / 14));
	case TEXT_TABLE_ROW_3:
		return static_cast<int>((s_screenSize.x / 14));

	default:
		return static_cast<int>((s_screenSize.x / len));
	}
}

const int Text::getFontYpos()
{
	switch (m_type)
	{
	case TEXT_VICTORY:
		return static_cast<int>((s_screenSize.y / 6) - (m_data.h / 2));
	case TEXT_PAUSE:
		return static_cast<int>((s_screenSize.y / 2) - (m_data.h / 1.5l));
	case TEXT_CONTINUE:
		return static_cast<int>((s_screenSize.y / 1.2l) - (m_data.h / 2));
	case TEXT_START:
		return static_cast<int>((s_screenSize.y / 1.5l) - (m_data.h / 2));
	case TEXT_WINS:
		return static_cast<int>((s_screenSize.y / 1.1l) - (m_data.h));
	case TEXT_CENTRE:
		return static_cast<int>((s_screenSize.y / 2) - (m_data.h / 1.5l));

	case TEXT_TABLE_ROW_0:
		return static_cast<int>((s_screenSize.y / 2.5l) - (m_data.h / 2));
	case TEXT_TABLE_ROW_1:
		return static_cast<int>((s_screenSize.y / 2.0l) - (m_data.h / 2));
	case TEXT_TABLE_ROW_2:
		return static_cast<int>((s_screenSize.y / 1.666666666l) - (m_data.h / 2));
	case TEXT_TABLE_ROW_3:
		return static_cast<int>((s_screenSize.y / 1.428571429l) - (m_data.h /2));

	default:
		return static_cast<int>((s_screenSize.y / 3));
	}
}

void Text::doFontStuff(SDL_Window *win, SDL_Renderer *ren)
{
	m_font = TTF_OpenFont(s_fontPath.c_str(), m_fontSize);

	if (m_font == nullptr) SDLError("OpenFont", win, ren);

	SDL_Surface *surface{ TTF_RenderUTF8_Blended(m_font, m_message.c_str(), m_col) };

	if (surface == nullptr) SDLError("RenderUTF8_Blended", win, ren);
	else
	{
		m_tex = SDL_CreateTextureFromSurface(ren, surface);

		if (m_tex == nullptr)
		{
			SDL_DestroyTexture(m_tex);
			SDLError("CreateTextureFromSurface", win, ren);
		}
		else
		{
			m_data.w = surface->w;
			m_data.h = surface->h;
		}
		SDL_FreeSurface(surface);
	}
}

void Text::change(SDL_Window *win, SDL_Renderer *ren, const std::string m, const Usage mT, SDL_Color col, char xpos)
{
	m_message	= m;
	m_type		= mT;
	m_fontSize	= getFontSize(m, mT);
	m_col		= col;

	doFontStuff(win, ren);

	if (xpos == 'C') //i.e. default
		m_data.x = ((s_screenSize.x / 2) - (m_data.w / 2));
	else if (xpos == 'L') // LEFT aligned
		m_data.x = static_cast<int>(s_screenSize.x / 5);
	else if (xpos == 'R') // RIGHT aligned
		m_data.x = static_cast<int>((s_screenSize.x / 1.3l) - (m_data.w));
	else
		m_data.x = 0;

	m_data.y	= getFontYpos();
}

Text::Text(SDL_Window *win, SDL_Renderer *ren, const std::string m, const Usage mT, SDL_Color col)
{
	change(win, ren, m, mT, col);
}

void Text::render(SDL_Window *win, SDL_Renderer *ren)
{
	if (SDL_RenderCopy(ren, m_tex, NULL, &m_data))
	{
		SDL_DestroyTexture(m_tex);
		SDLError("RenderCopy [Text::render()]", win, ren);
	}
}

Text::~Text()
{
	SDL_DestroyTexture(m_tex);
	TTF_CloseFont(m_font);
}


// "Blue Wins"
// "Orange Wins"
// "Green Wins"
// "Purple Wins"
// "No One Wins"

// "PAUSED"