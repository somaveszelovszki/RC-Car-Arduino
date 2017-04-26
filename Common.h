#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include <stdlib.h>

class Common {
public:

	enum ERROR_SIGN {
		POSITIVE,
		NEGATIVE,
		BOTH
	};

	static size_t write(Print& printer, const String& str);

	static void initTimer();

	template <typename T>
	static void arrayCopy(T dest[], T src[], size_t size);

	static uint64_t MILLI_SEC_COUNTER;

	static uint64_t milliSecs();

	static bool contains(char array[], size_t arraySize, char item);
};

#endif
