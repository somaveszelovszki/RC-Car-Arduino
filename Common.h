#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include <stdlib.h>

class Common {
public:

	/*
		If you add a position, update POSITION_COUNT
	*/
	enum POSITION {
		FRONT_LEFT = 0,
		FRONT_RIGHT = 1,
		BACK_LEFT = 2,
		BACK_RIGHT = 3,
		LEFT = 4,
		RIGHT = 5
	};
	static const unsigned int POSITION_COUNT = 6;

	static size_t write(Print& printer, const String& str);

	static void initTimer();

	template <typename T>
	static void arrayCopy(T dest[], T src[], size_t size);
};

#endif
