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

int32_t Common::bytesToInt(const byte bytes[], int startIndex = 0) {
	//return *reinterpret_cast<const int32_t*>(&bytes[startIndex]);
	return (static_cast<int32_t>(bytes[startIndex]) << 24)
		| (static_cast<int32_t>(bytes[startIndex + 1]) << 16)
		| (static_cast<int32_t>(bytes[startIndex + 2]) << 8)
		| static_cast<int32_t>(bytes[startIndex + 3]);
}

float Common::bytesToFloat(const byte bytes[], int startIndex = 0) {
	// TODO
	return 0.0f;
	//return *reinterpret_cast<const float*>(&bytes[startIndex]);
}

void Common::intToBytes(int32_t value, byte dest[4]) {
	//*reinterpret_cast<int32_t*>(dest) = value;
	dest[0] = static_cast<byte>(value >> 24);
	dest[1] = static_cast<byte>(value >> 16);
	dest[2] = static_cast<byte>(value >> 8);
	dest[3] = static_cast<byte>(value);
}

void Common::floatToBytes(float value, byte dest[4]) {
	//*reinterpret_cast<float*>(dest) = value;
	// TODO
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
