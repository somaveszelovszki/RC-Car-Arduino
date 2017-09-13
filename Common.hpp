#ifndef COMMON_HPP
#define COMMON_HPP

#include <Arduino.h>
#include <Print.h>
#include <math.h>

#include "def.hpp"

#define __DEBUG true

#define radToDeg(deg) ((deg) * (DEG_TO_RAD))
#define degToRad(rad) ((rad) * (RAD_TO_DEG))

#define dirAngleToXY(angle) ((angle) + M_PI_2)
#define XYAngleToDir(angle) ((angle) - M_PI_2)

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

	/** @ brief Ultrasonic sensor positions.
	NOTE: If you add a position, don't forget to update ULTRA_NUM_SENSORS!
	*/
	enum UltrasonicPos {
		FRONT_RIGHT_CORNER = 0,
		FRONT_RIGHT,
		FRONT_LEFT,
		FRONT_LEFT_CORNER,

		LEFT_FRONT,
		LEFT_REAR,
		
		REAR_LEFT_CORNER,
		REAR_LEFT,
		REAR_RIGHT,
		REAR_RIGHT_CORNER,

		RIGHT_REAR,
		RIGHT_FRONT
	};

	struct ValidationData {
		int validationSampleNum;
		float relativeError;
	};

	static int write(Print& printer, const String& str);

	static void initializeTimer();

	static unsigned long milliSecs() {
		return MILLIS;
	}

	static void incrementMilliSecs() {
		++MILLIS;
	}

	template <typename T>
	static bool contains(const T array[], int arraySize, T item);

	template <typename T>
	static T pythagoreanHypotenuse(T a, T b);

	template <typename T>
	static bool isBetween(T value, T boundaryLow, T boundaryHigh);

	template <typename T>
	static T incarcerate(T value, T boundaryLow, T boundaryHigh);

	template <typename T>
	static void arrayCopy(const T src[], T dest[], int size);

	/*
	Checks if value is in a given range of the reference value.
	*/
	template <typename T>
	static bool isInRange(T ref, T value, float relativeError, ErrorSign errorDir = Common::ErrorSign::BOTH);
};

#endif	// COMMON_HPP
