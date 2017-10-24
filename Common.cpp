#include "Common.hpp"

using namespace rc_car;

unsigned long Common::MILLIS = 0;

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

int Common::bytesToInt(const byte bytes[], int startIndex = 0) {
	return (bytes[startIndex] << 24)
		| (bytes[startIndex + 1] << 16)
		| (bytes[startIndex + 2] << 8)
		| bytes[startIndex + 3];
}

float Common::bytesToFloat(const byte bytes[], int startIndex = 0) {
	const float *floatPtr = reinterpret_cast<const float*>(&bytes[startIndex]);
	return *floatPtr;
}

byte* Common::intToBytes(int value, byte dest[4]) {
	dest[0] = static_cast<byte>(value >> 24);
	dest[1] = static_cast<byte>(value >> 16);
	dest[2] = static_cast<byte>(value >> 8);
	dest[3] = static_cast<byte>(value);
}

byte* Common::floatToBytes(int value, byte dest[4]) {
	byte res[4];
	const byte *bytePtr = reinterpret_cast<const byte*>(&value);
	Common::arrayCopy<4>(bytePtr, res);
	return res;
}

void Common::debug_print(const String& str, bool addNewLine = false) {
	uint8_t en = digitalRead(COMM_EN_PIN);
	digitalWrite(COMM_EN_PIN, !COMM_EN_SIGNAL_LEVEL);
	addNewLine ? Serial.println(str) : Serial.print(str);
	digitalWrite(COMM_EN_PIN, en);
}

void Common::debug_println(const String& str) {
	debug_print(str, true);
}

bool rc_car::Common::testAndSet(bool *value, bool valueToSet = true) {
	noInterrupts();
	bool res = *value;
	*value = valueToSet;
	interrupts();
	return res;
}
