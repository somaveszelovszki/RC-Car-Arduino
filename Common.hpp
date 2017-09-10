#ifndef COMMON_HPP
#define COMMON_HPP

#include <Arduino.h>
#include <math.h>

#include "def.hpp"

#define __DEBUG true

#define radToDeg(deg) ((deg) * (DEG_TO_RAD))
#define degToRad(rad) ((rad) * (RAD_TO_DEG))

#define SEC_TO_MSEC 1000

class Common {

private:
	static unsigned long MILLIS;

public:
	/*
	Drive mode defines if the program should override user's commands.
	*/
	enum DriveMode {
		FREE_DRIVE = 1,		// user has total control, no override
		SAFE_DRIVE = 2,		// user has limited control, crash avoidance
		AUTOPILOT = 3		// user has no control, program drives
	};

	enum ErrorSign {
		POSITIVE,
		NEGATIVE,
		BOTH
	};

	enum AccelerationDir {
		FORWARD = 1,
		BACKWARD = -1,
		RELEASE = 0
	};

	enum SteeringDir {
		LEFT = 1,
		RIGHT = -1
	};

	/*
	NOTE: If you add a position, don't forget to update ULTRA_NUM_SENSORS!
	*/
	enum UltrasonicPos {
		FRONT_LEFT_CORNER = 0,
		FRONT_LEFT = 1,
		FRONT_RIGHT = 2,
		FRONT_RIGHT_CORNER = 3,

		REAR_LEFT_CORNER = 4,
		REAR_LEFT = 5,
		REAR_RIGHT = 6,
		REAR_RIGHT_CORNER = 7,

		LEFT_FRONT = 8,
		LEFT_REAR = 9,
		RIGHT_FRONT = 10,
		RIGHT_REAR = 11
	};

	struct ValidationData {
		int validationSampleNum;
		float relativeError;
	};

	static int write(Print& printer, const String& str);

	static void initializeTimer();

	template <typename T>
	static void arrayCopy(T dest[], T src[], int size);

	static unsigned long milliSecs() {
		return MILLIS;
	}

	static void incrementMilliSecs() {
		++MILLIS;
	}

	template <typename T>
	static bool contains(const T array[], int arraySize, T item);

	template <typename T, typename R = T>
	static R pythagoreanHypotenuse(T a, T b);

	template <typename T>
	static bool isBetween(T value, T boundary1, T boundary2);

	/*
	Checks if value is in a given range of the reference value.
	*/
	template <typename T>
	static bool isInRange(T ref, T value, float relativeError, ErrorSign errorDir = Common::ErrorSign::BOTH);
};

#endif	// COMMON_HPP
