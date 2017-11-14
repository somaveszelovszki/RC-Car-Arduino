#ifndef RC_CAR__COMMON__HPP
#define RC_CAR__COMMON__HPP

#include <math.h>

#include "config.hpp"

namespace rc_car {

#define dirAngleToXY(angle) ((angle) + M_PI_2)
#define XYAngleToDir(angle) ((angle) - M_PI_2)

#define SEC_TO_MSEC 1000

#if __DEBUG
#define DEBUG_print(str) Serial.print(str)
#define DEBUG_println(str) Serial.println(str)
#endif // __DEBUG


	/** @brief Common functions and enumerations that are used throughout the project.
	*/
	class Common {

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
			RIGHT_FRONT = 0,

			FRONT_RIGHT_CORNER,
			FRONT_RIGHT,
			FRONT_LEFT,
			FRONT_LEFT_CORNER,

			LEFT_FRONT,
			LEFT_REAR,

			REAR_LEFT_CORNER,
			REAR_LEFT,
			REAR_RIGHT,
			REAR_RIGHT_CORNER,

			RIGHT_REAR
		};

		static UltrasonicPos nextUltrasonicPos(UltrasonicPos pos) {
			return static_cast<UltrasonicPos>((static_cast<int>(pos) + 1) % ULTRA_NUM_SENSORS);
		}

		/** @brief Calculates section start position according to forward direction.

		For given forward directions the section start positions are the following:

		RIGHT_FRONT:
		FRONT_RIGHT_CORNER:
			sectionStartPos = RIGHT_FRONT;

		FRONT_RIGHT:
		FRONT_LEFT:
			sectionStartPos = FRONT_RIGHT_CORNER;

		FRONT_LEFT_CORNER:
		LEFT_FRONT:
			sectionStartPos = FRONT_RIGHT;

		LEFT_REAR:
		REAR_LEFT_CORNER:
			sectionStartPos = LEFT_REAR;

		REAR_LEFT:
		REAR_RIGHT:
			sectionStartPos = REAR_LEFT_CORNER;

		REAR_RIGHT_CORNER:
		RIGHT_REAR:
			sectionStartPos = REAR_LEFT

		@param fwdPos The forward direction.
		*/
		static UltrasonicPos calculateSectionStartPos(UltrasonicPos fwdPos) {
			return static_cast<UltrasonicPos>(static_cast<int>(fwdPos) >= 6 ? 3 + static_cast<int>(fwdPos) / 2 : static_cast<int>(fwdPos) / 2);
		}

		struct Validation {
			int minSampleNum;	// minimum validation sample num
			float relErr;		// relative error
		};

		static void initializeTimer();

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
		static bool isInRange(T ref, T value, float relErr, ErrorSign errorDir = Common::ErrorSign::BOTH);

		template <int size1, int size2, typename T>
		static void arrayConcat(const T ar1[], const T ar2[], T res[]);

		static int32_t bytesToInt(const byte bytes[], int startIndex = 0);

		static float bytesToFloat(const byte bytes[], int startIndex = 0);

		static void intToBytes(int32_t value, byte dest[4]);

		static void floatToBytes(float value, byte dest[4]);

		static bool testAndSet(bool *value, bool valueToSet = true);

		template <typename T>
		static T map(T value, T fromLow, T fromHigh, T toLow, T toHigh);
	};

	template<typename T>
	inline bool Common::isInRange(T ref, T value, float relErr, ErrorSign errorDir) {
		T max = errorDir == ErrorSign::NEGATIVE ? ref : static_cast<T>(ref * (1.0F + relErr));
		T min = errorDir == ErrorSign::POSITIVE ? ref : static_cast<T>(ref * (1.0F - relErr));
		return isBetween(value, min, max);
	}

	template<int size1, int size2, typename T>
	inline void Common::arrayConcat(const T ar1[], const T ar2[], T res[]) {
		Common::arrayCopy<size1>(ar1, res);
		Common::arrayCopy<size2>(ar2, &res[size1]);
	}

	template<typename T>
	inline T Common::map(T value, T fromLow, T fromHigh, T toLow, T toHigh) {
		if (value <= fromLow)
			return toLow;

		if (value >= fromHigh)
			return toHigh;

		return toLow + (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
	}

}
#endif // RC_CAR__COMMON__HPP
