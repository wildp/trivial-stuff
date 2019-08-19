// text.h
// contains declarations for the Text class

#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

class Text
{
	static std::string s_fontPath;
	static SDL_Point s_screenSize;

public:
	enum Usage
	{
		TEXT_GENERAL,
		TEXT_VICTORY,
		TEXT_PAUSE,
		TEXT_CONTINUE,
		TEXT_START,
		TEXT_WINS,
		TEXT_CENTRE,
		TEXT_TABLE_ROW_0,
		TEXT_TABLE_ROW_1,
		TEXT_TABLE_ROW_2,
		TEXT_TABLE_ROW_3,
		TEXT_NONE,
	};

private:
	std::string m_message;
	Usage m_type;
	int m_fontSize;

	TTF_Font *m_font;
	SDL_Texture *m_tex;
	SDL_Color m_col;
	SDL_Rect m_data;
	

	const int getFontSize(std::string message, Usage type);

	const int getFontYpos();

	void doFontStuff(SDL_Window *win, SDL_Renderer *ren);

public:

	static void setFont(std::string PATH = " ");

	static void setScreenSize(const SDL_Point &SCREEN);

	static Usage getUsage(const int row);

	Text(SDL_Window *win, SDL_Renderer *ren, const std::string m= " ", const Usage mT = Text::TEXT_NONE, SDL_Color col = { Game::text[0], Game::text[1], Game::text[2] });

	void change(SDL_Window *win, SDL_Renderer *ren, const std::string m, const Usage mT, SDL_Color col = { Game::text[0], Game::text[1], Game::text[2] }, char xpos = 'C');

	void render(SDL_Window *win, SDL_Renderer *ren);

	~Text();




};

#endif // !TEXT_H


