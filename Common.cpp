#include "Common.hpp"

int Common::write(Print& printer, const String& str) {

	int numPrintedBytes = 0;

	// prints all characters
	for (int i = 0; i < str.length(); i++) {
		numPrintedBytes += printer.print(str.charAt(i));
	}

	return numPrintedBytes;
}

void Common::initTimer() {
	noInterrupts();

	// Timer0 is already used for millis() - we'll just interrupt somewhere in the middle
	// and call the ISR(TIMER0_COMPA_vect) function defined in RC_Car.ino
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);

	interrupts();
}

bool Common::contains(char array[], int arraySize, char item) {
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
static void Common::arrayCopy(T dest[], T src[], int size) {
	for (int i = 0; i < size; ++i) {
		dest[i] = src[i];
	}
}

template <typename T>
bool Common::isInRange(T ref, T value, float relativeError, Common::ERROR_SIGN errorDir) {

	T max = errorDir == ERROR_SIGN::NEGATIVE ?
		ref : static_cast<T>(ref * (1.0F + relativeError));

	T min = errorDir == ERROR_SIGN::POSITIVE ?
		ref : static_cast<T>(ref * (1.0F - relativeError));

	return isBetween(value, min, max);
}

unsigned long Common::MILLIS = 0;

