#ifndef COMMON_HPP
#define COMMON_HPP

#include <Arduino.h>
#include <math.h>

#include "def.hpp"

class Common {

private:
	static unsigned long MILLIS;

public:
	enum ERROR_SIGN {
		POSITIVE,
		NEGATIVE,
		BOTH
	};

	enum AccelerationDir {
		FORWARD, BACKWARD, RELEASE
	};

	enum SteeringDir {
		LEFT = 1,
		RIGHT = -1
	};

	struct ValidationData {
		int validationSampleNum;
		float relativeError;
	};

	static int write(Print& printer, const String& str);

	static void initTimer();

	template <typename T>
	static void arrayCopy(T dest[], T src[], int size);

	static unsigned long milliSecs() {
		return MILLIS;
	}

	static void incrementMilliSecs() {
		++MILLIS;
	}

	static bool contains(char array[], int arraySize, char item);

	static double degreeToRadian(double degree);
	static double radianToDegree(double radian);

	template <typename T>
	static double pythagoreanHypotenuse(T a, T b);

	template <typename T>
	static bool isBetween(T value, T boundary1, T boundary2);

	/*
	Checks if value is in a given range of the reference value.
	*/
	template <typename T>
	static bool isInRange(T ref, T value, float relativeError, ERROR_SIGN errorDir);
};

#endif	// COMMON_HPP
