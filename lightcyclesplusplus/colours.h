// colours.h
// stores player colours and their names

#ifndef COLOURS_H
#define COLOURS_H

namespace Game
{
	constexpr Uint8 colours[4][3]
	{	//format: RGB (A = 255, constant)
		{   0, 191, 255 },	/* Blue */
		{ 255, 127,   0 },	/* Orange */
		{   0, 191,   0 },	/* Green */
		{ 191,   0, 255 }	/* Purple */
	};

	constexpr Uint8 background[3]
	{   0,   0,   0 };
	constexpr Uint8 text[3]
	{ 255, 255, 255 };
	constexpr Uint8 pause[4]
	{ 128, 128, 128 ,128 };
	constexpr Uint8 box[3]
	{ 64, 64, 64 };

	const std::string colourNames[4]
	{
		"Blue",
		"Orange",
		"Green",
		"Purple",
	};
}

#endif // !COLORS_H

