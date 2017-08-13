#include "Common.hpp"

size_t Common::write(Print& printer, const String& str) {

	size_t numPrintedBytes = 0;

	// prints all characters
	for (size_t i = 0; i < str.length(); i++) {
		numPrintedBytes += printer.print(str.charAt(i));
	}

	return numPrintedBytes;
}

void Common::initTimer() {
	noInterrupts();

	// Timer0 is already used for millis() - we'll just interrupt somewhere
	// in the middle and call the "Compare A" function below
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);

	interrupts();
}

uint64_t Common::milliSecs() {
	return MILLI_SEC_COUNTER;
}

bool Common::contains(char array[], size_t arraySize, char item) {
	for (int i = 0; i < arraySize; ++i) {
		if (array[i] == item)
			return true;
	}
	return false;
}

double Common::degreeToRadian(double degree) {
	return degree * DEG_TO_RAD;

}

double Common::radianToDegree(double radian) {
	return radian * RAD_TO_DEG;
}

template <typename T>
double Common::pythagoreanHypotenuse(T a, T b) {
	return sqrt(static_cast<double>(a) * static_cast<double>(a) + static_cast<double>(b) * static_cast<double>(b));
}

template<typename T>
bool Common::isBetween(T value, T boundary1, T boundary2) {
	return boundary2 >= boundary1 ? value >= boundary1 && value <= boundary2
		: value >= boundary2 && value <= boundary1;
}

template <typename T>
static void Common::arrayCopy(T dest[], T src[], size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dest[i] = src[i];
	}
}

uint64_t Common::MILLI_SEC_COUNTER = 0;

