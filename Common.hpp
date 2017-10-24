#ifndef COMMON_HPP
#define COMMON_HPP

#include <math.h>

#include "config.hpp"

namespace rc_car {

#define dirAngleToXY(angle) ((angle) + M_PI_2)
#define XYAngleToDir(angle) ((angle) - M_PI_2)

#define SEC_TO_MSEC 1000

	/** @brief Common functions and enumerations that are used throughout the project.
	*/
	class Common {

	private:
		static unsigned long MILLIS;

	public:
		/*
		Drive mode defines if the program should override user's messages.
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

		static UltrasonicPos nextUltrasonicPos(UltrasonicPos pos) {
			return static_cast<UltrasonicPos>((static_cast<int>(pos) + 1) % ULTRA_NUM_SENSORS);
		}

		struct ValidationData {
			int validationSampleNum;
			float relativeError;
		};

		static void initializeTimer();

		static unsigned long milliSecs() {
			return MILLIS;
		}

		static void incrementMilliSecs() {
			++MILLIS;
		}

		template <typename T>
		static bool contains(const T array[], int arraySize, T item) {
			bool c = false;
			for (int i = 0; i < arraySize; ++i)
				if (array[i] == item)
					c = true;
			return c;
		}

		template <typename T>
		static T pythagoreanHypotenuse(T a, T b) {
			return static_cast<T>(sqrt(static_cast<float>(a * a + b * b)));
		}

		template <typename T>
		static bool isBetween(T value, T boundaryLow, T boundaryHigh) {
			return boundaryHigh >= boundaryLow ? value >= boundaryLow && value <= boundaryHigh
				: value >= boundaryHigh && value <= boundaryLow;
		}

		template <typename T>
		static T incarcerate(T value, T boundaryLow, T boundaryHigh) {
			return min(max(value, boundaryLow), boundaryHigh);
		}

		template <int size, typename T>
		static void arrayCopy(const T src[], T dest[]) {
			for (int i = 0; i < size; ++i)
				dest[i] = src[i];
		}

		/*
		Checks if value is in a given range of the reference value.
		*/
		template <typename T>
		static bool isInRange(T ref, T value, float relativeError, ErrorSign errorDir = Common::ErrorSign::BOTH) {
			T max = errorDir == ErrorSign::NEGATIVE ? ref : static_cast<T>(ref * (1.0F + relativeError));
			T min = errorDir == ErrorSign::POSITIVE ? ref : static_cast<T>(ref * (1.0F - relativeError));
			return isBetween(value, min, max);
		}

		template <int size1, int size2, typename T>
		static void arrayConcat(const T ar1[], const T ar2[], T res[]) {
			Common::arrayCopy<size1>(ar1, res);
			Common::arrayCopy<size2>(ar2, &res[size1]);
		}

		static int bytesToInt(const byte bytes[], int startIndex = 0);

		static float bytesToFloat(const byte bytes[], int startIndex = 0);

		static byte* intToBytes(int value, byte dest[4]);

		static byte* floatToBytes(int value, byte dest[4]);

		static void debug_print(const String& str, bool addNewLine = false);
		static void debug_println(const String& str);

		static bool testAndSet(bool *value, bool valueToSet = true);
	};

}
#endif	// COMMON_HPP
