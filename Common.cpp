#include "Common.hpp"

using namespace rc_car;

void Common::initializeTimer() {
	//noInterrupts();

	// TODO check if this clashes with Servo!!!

	// Timer0 is already used for millis() - we'll just interrupt somewhere in the middle
	// and call the ISR(TIMER0_COMPA_vect) function defined in RC_Car.ino
	/*OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);*/

	OCR1B = 0x3F;
	TIMSK1 |= _BV(OCIE1B);

	//interrupts();
}

int32_t Common::bytesToInt(const byte bytes[], int startIndex = 0) {
	return (static_cast<int32_t>(bytes[startIndex]) << 24)
		| (static_cast<int32_t>(bytes[startIndex + 1]) << 16)
		| (static_cast<int32_t>(bytes[startIndex + 2]) << 8)
		| static_cast<int32_t>(bytes[startIndex + 3]);
	//return *reinterpret_cast<const int*>(&bytes[startIndex]);
}

float Common::bytesToFloat(const byte bytes[], int startIndex = 0) {
	int32_t intVal = bytesToInt(bytes, startIndex);
	return *reinterpret_cast<float*>(&intVal);
	//return *reinterpret_cast<const float*>(&bytes[startIndex]);
}

void Common::intToBytes(int32_t value, byte dest[4]) {
	dest[0] = static_cast<byte>(value >> 24);
	dest[1] = static_cast<byte>(value >> 16);
	dest[2] = static_cast<byte>(value >> 8);
	dest[3] = static_cast<byte>(value);

	//arrayCopy<4>(reinterpret_cast<byte*>(&value), dest);
}

void Common::floatToBytes(float value, byte dest[4]) {
	int32_t intVal = *reinterpret_cast<int32_t*>(&value);
	intToBytes(intVal, dest);

	//arrayCopy<4>(reinterpret_cast<byte*>(&value), dest);
}

bool Common::testAndSet(bool *value, bool valueToSet = true) {
	noInterrupts();
	bool res = *value;
	*value = valueToSet;
	interrupts();
	return res;
}
