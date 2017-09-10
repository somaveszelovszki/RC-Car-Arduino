#include "Common.hpp"

int Common::write(Print& printer, const String& str) {

	int numPrintedBytes = 0;

	// prints all characters
	for (int i = 0; i < str.length(); i++) {
		numPrintedBytes += printer.print(str.charAt(i));
	}

	return numPrintedBytes;
}

void Common::initializeTimer() {
	noInterrupts();

	// Timer0 is already used for millis() - we'll just interrupt somewhere in the middle
	// and call the ISR(TIMER0_COMPA_vect) function defined in RC_Car.ino
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);

	interrupts();
}

template <typename T>
bool Common::contains(const T array[], int arraySize, T item) {
	for (int i = 0; i < arraySize; ++i) {
		if (array[i] == item)
			return true;
	}
	return false;
}

template <typename T, typename R = T>
R Common::pythagoreanHypotenuse(T a, T b) {
	return static_cast<R>(sqrtf(static_cast<float>(a * a + b * b));
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
bool Common::isInRange(T ref, T value, float relativeError, Common::ErrorSign errorDir = Common::ErrorSign::BOTH) {

	T max = errorDir == ErrorSign::NEGATIVE ?
		ref : static_cast<T>(ref * (1.0F + relativeError));

	T min = errorDir == ErrorSign::POSITIVE ?
		ref : static_cast<T>(ref * (1.0F - relativeError));

	return isBetween(value, min, max);
}

unsigned long Common::MILLIS = 0;

