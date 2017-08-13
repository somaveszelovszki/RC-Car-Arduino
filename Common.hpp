#ifndef COMMON_HPP
#define COMMON_HPP

#include <Arduino.h>
#include <math.h>

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

	static double degreeToRadian(double degree);
	static double radianToDegree(double radian);

	template <typename T>
	static double pythagoreanHypotenuse(T a, T b);

	template <typename T>
	static bool isBetween(T value, T boundary1, T boundary2);
};

#endif	// COMMON_HPP
