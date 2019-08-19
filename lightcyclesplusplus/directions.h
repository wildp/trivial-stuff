// directions.h
// defines direction related stuff

#ifndef ENUM_DIRECTIONS_H
#define ENUM_DIRECTIONS_H

enum class Directions
{
	NONE = -1,
	UP,
	LEFT,
	DOWN,
	RIGHT,
	MAX_DIRECTIONS,
};

#endif // ENUM_DIRECTIONS_H

#ifndef START_DIRECTIONS_H
#define START_DIRECTIONS_H

namespace Game
{
	constexpr Directions startDir[4]
	{
		Directions::RIGHT,
		Directions::LEFT,
		Directions::DOWN,
		Directions::UP
	};
}

#endif // !START_DIRECTIONS_H





