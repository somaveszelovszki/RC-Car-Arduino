#ifndef RC_CAR__COMMON__HPP
#define RC_CAR__COMMON__HPP

#include <math.h>

#include "config.hpp"

/** @brief Common macros that are used throughout the project.
*/

// Converts direction angle to XY coordinate system angle
#define dirAngleToXY(angle) ((angle) + M_PI_2)

// Converts XY coordinate system angle to direction angle
#define XYAngleToDir(angle) ((angle) - M_PI_2)

// Converts sec to millisec
#define SEC_TO_MSEC 1000

#if __DEBUG
// Prints string to Serial port
#define DEBUG_print(str) Serial.print(str)

// Prints string to Serial port with endline character
#define DEBUG_println(str) Serial.println(str)
#endif // __DEBUG

namespace rc_car {

    /** @brief Common functions and enumerations that are used throughout the project.
    */
    class Common {

    public:
        /** @brief Drive mode defines the working mechanism of the DriveTask.
        */
        enum DriveMode {
            FREE_DRIVE = 1,		// user has total control, no override
            SAFE_DRIVE = 2,		// user has limited control, crash avoidance
            AUTOPILOT = 3		// user has no control, program drives
        };

        /** @brief Defines steering direction.
        */
        enum SteeringDir {
            LEFT = 1,
            RIGHT = -1
        };

        /** @ brief Ultrasonic sensor positions.
        NOTE: The values are also used as array indexes!
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

        /** @brief Calculates next ultrasonic sensor postion in the line - used for iterating through all sensors.

        @param pos The current ultrasonic sensor position.
        @returns the next ultrasonic sensor position in the line.
        */
        static UltrasonicPos nextUltrasonicPos(UltrasonicPos pos) {
            return static_cast<UltrasonicPos>((static_cast<int>(pos) + 1) % ULTRA_NUM_SENSORS);
        }

        /** @brief Calculates section start position according to forward direction.

        For given forward directions the section start positions are the following:

        RIGHT_FRONT and
        FRONT_RIGHT_CORNER:
        sectionStartPos = RIGHT_FRONT;

        FRONT_RIGHT and
        FRONT_LEFT:
        sectionStartPos = FRONT_RIGHT_CORNER;

        FRONT_LEFT_CORNER and
        LEFT_FRONT:
        sectionStartPos = FRONT_RIGHT;

        LEFT_REAR and
        REAR_LEFT_CORNER:
        sectionStartPos = LEFT_REAR;

        REAR_LEFT and
        REAR_RIGHT:
        sectionStartPos = REAR_LEFT_CORNER;

        REAR_RIGHT_CORNER and
        RIGHT_REAR:
        sectionStartPos = REAR_LEFT

        @param fwdPos The forward direction.
        */
        static UltrasonicPos calculateSectionStartPos(UltrasonicPos fwdPos) {
            return static_cast<UltrasonicPos>(static_cast<int>(fwdPos) >= 6 ? 3 + static_cast<int>(fwdPos) / 2 : static_cast<int>(fwdPos) / 2);
        }

        /** @brief Defines sign of error between value and reference - used when determining whether a value is within the given range of a reference.
        */
        enum ErrorSign {
            POSITIVE,
            NEGATIVE,
            BOTH
        };

        /** @brief Contains validation data - hepls when validating values according to previous values.
        */
        struct Validation {
            int minSampleNum;	// minimum validation sample num
            float relErr;		// relative error
        };

        /** @brief If the array contains the item, return its index, otherwise returns -1.

        @tparam T Type of the item and the elements of the array.
        @param item The item to look for.
        @param _array The array in which the check should be performed.
        @param arraySize The size of the array.
        @returns The index of the item or -1 if the array does not contain the item.
        */
        template <typename T>
        static int indexOf(T item, const T array[], int arraySize) {
            int idx = -1;
            for (int i = 0; idx == -1 && i < arraySize; ++i)
                if (array[i] == item)
                    idx = i;
            return idx;
        }

        /** @brief Checks if the array contains the given item.

        @tparam T Type of the item and the elements of the array.
        @param item The item to look for.
        @param _array The array in which the check should be performed.
        @param arraySize The size of the array.
        @returns Boolean value indicating if the array contains the item.
        */
        template <typename T>
        static bool contains(T item, const T _array[], int arraySize) {
            return indexOf(item, _array, arraySize) != -1;
        }

        /** @brief Calculates the hypotenuse of a triangle using the Pythagorean theory.

        @param a The length of one leg of the triangle.
        @param b The length of the other leg of the tri angle.
        @returns The length of the hypotenuse of the triangle.
        */
        template <typename T>
        static T pythagoreanHypotenuse(T a, T b) {
            return static_cast<T>(sqrt(static_cast<float>(a * a + b * b)));
        }

        /** @brief Checks if a value is between the given boundaries.

        @tparam T Type of the elements.
        @param value The value to check.
        @param boundaryLow The lower boundary.
        @param boundaryHigh The higher boundary.
        @returns Boolean value indicating if the value is between the boundaries.
        */
        template <typename T>
        static bool isBetween(T value, T boundaryLow, T boundaryHigh) {
            return boundaryHigh >= boundaryLow ? value >= boundaryLow && value <= boundaryHigh
                : value >= boundaryHigh && value <= boundaryLow;
        }

        /** @brief Incarcerates value between the given boundaries.

        @tparam T Type of the elements.
        @param value The value to incarcerate.
        @param boundaryLow The lower boundary.
        @param boundaryHigh The higher boundary.
        @returns The incarcerated value.
        */
        template <typename T>
        static T incarcerate(T value, T boundaryLow, T boundaryHigh) {
            return min(max(value, boundaryLow), boundaryHigh);
        }

        /** @brief Copies elements of the source array into the destination array.

        @tparam size Size of the arrays.
        @tparam T Type of the elements of the arrays.
        @param src The source array.
        @param dest The destination array.
        */
        template <int size, typename T>
        static void arrayCopy(const T src[], T dest[]);

        /** @brief Checks if value is in a given range of the reference value.

        @tparam T Numeric type of the value and the reference.
        @param value The value to compare to the reference.
        @param ref The reference value.
        @param relErr The permitted relative error.
        @param errorSign The sign of the permitted error (determines if value must be smaller/larger than the reference) - ErrorSign::BOTH by default.
        */
        template <typename T>
        static bool isInRange(T value, T ref, float relErr, ErrorSign errorSign = ErrorSign::BOTH);

        /** @brief Concatenates two arrays.

        @tparam size1 Size of the first array.
        @tpara size2 Size of the second array.
        @tparam T Numeric type of the arrays.
        @param ar1 The first array.
        @param ar2 The second array.
        @param res The result array.
        */
        template <int size1, int size2, typename T>
        static void arrayConcat(const T ar1[], const T ar2[], T res[]);

        /** @brief Converts byte array to integer.

        @param bytes The byte array.
        @param startIndex The start index of the conversion - integer value is stored in bytes [startIndex, startIndex + 3].
        @returns The integer value.
        */
        static int32_t bytesToInt(const byte bytes[], int startIndex = 0);

        /** @brief Converts byte array to float.

        @param bytes The byte array.
        @param startIndex The start index of the conversion - float value is stored in bytes [startIndex, startIndex + 3].
        @returns The float value.
        */
        static float bytesToFloat(const byte bytes[], int startIndex = 0);

        /** @brief Converts integer value to byte array.

        @param value The integer value.
        @param dest The destination byte array.
        */
        static void intToBytes(int32_t value, byte dest[4]);

        /** @brief Converts float value to byte array.

        @param value The float value.
        @param dest The destination byte array.
        */
        static void floatToBytes(float value, byte dest[4]);

        /** @brief Sets binary variable to given value and returns its previous state.
        NOTE: Interrupt are disabled during the operation, so it is atomic!

        @param pValue Pointer to the binary value.
        @param valueToSet The binary value to set - TRUE by default.
        */
        static bool testAndSet(bool *pValue, bool valueToSet = true) {
            noInterrupts();
            bool res = *pValue;
            *pValue = valueToSet;
            interrupts();
            return res;
        }

        /** @brief Maps value from from a given range to another.

        @tparam T Numeric type of the value and the range boundaries.
        @param value The value to map.
        @param fromLow Lower boundary of the source range.
        @param fromHigh Higher boundary of the source range.
        @param toLow Lower boundary of the destination range.
        @param toHigh Higher boundary of the destination range.
        @returns The mapped value.
        */
        template <typename T>
        static T map(T value, T fromLow, T fromHigh, T toLow, T toHigh);

        /** @brief Checks if given value equals the reference with the given epsilon tolerance.

        @tparam T Numeric type of the value, the reference and the epsilon tolerance.
        @param value The value to compare to the reference.
        @param ref The reference.
        @param eps The epsilon tolerance - 0.0001f by default.
        */
        template <typename T>
        static bool areEqual(T value, T ref, T eps = static_cast<T>(0.0001f)) {
            return (value >= ref - eps) && (value <= ref + eps);
        }
    };

    template <int size, typename T>
    void Common::arrayCopy(const T src[], T dest[]) {
        for (int i = 0; i < size; ++i)
            dest[i] = src[i];
    }

    template<typename T>
    bool Common::isInRange(T value, T ref, float relErr, Common::ErrorSign errorSign = Common::ErrorSign::BOTH) {
        T _max = errorSign == ErrorSign::NEGATIVE ? ref : static_cast<T>(ref * (1.0F + relErr));
        T _min = errorSign == ErrorSign::POSITIVE ? ref : static_cast<T>(ref * (1.0F - relErr));
        return Common::isBetween(value, _min, _max);
    }

    template <int size1, int size2, typename T>
    void Common::arrayConcat(const T ar1[], const T ar2[], T res[]) {
        Common::arrayCopy<size1>(ar1, res);
        Common::arrayCopy<size2>(ar2, &res[size1]);
    }

    template <typename T>
    T Common::map(T value, T fromLow, T fromHigh, T toLow, T toHigh) {
        if (value <= fromLow)
            return toLow;

        if (value >= fromHigh)
            return toHigh;

        return toLow + (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
    }

}
#endif // RC_CAR__COMMON__HPP