#include "RotaryEncoder.h"

RotaryEncoder *motorRotaryEncoder = new RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);

RotaryEncoder::RotaryEncoder(uint8_t A_pin, uint8_t B_pin) {
	this->A_pin = A_pin;
	this->B_pin = B_pin;
}

void RotaryEncoder::initialize() {
	pinMode(ROTARY_ENCODER_A_PIN, INPUT);
	pinMode(ROTARY_ENCODER_B_PIN, INPUT);
	// turn on pullup resistors
	digitalWrite(ROTARY_ENCODER_A_PIN, HIGH);
	digitalWrite(ROTARY_ENCODER_B_PIN, HIGH);

	// encoder pin on interrupt 0 (pin 2)
	attachInterrupt(0, doRotaryEncoderA, CHANGE);
	// encoder pin on interrupt 1 (pin 3)
	attachInterrupt(1, doRotaryEncoderB, CHANGE);

	watchdog->restart();

	position = 0;
	time = 0;
	storedResult.d_time = 0;
	storedResult.d_pos = 0;
}

void RotaryEncoder::onChange_A() {
	// Test transition, did things really change? 
	if (digitalRead(ROTARY_ENCODER_A_PIN) != A_set) {  // debounce once more
		A_set = !A_set;

		// adjust counter + if A leads B
		if (A_set && !B_set)
			++position;
		//Serial.println(position);
	}
}

void RotaryEncoder::onChange_B() {
	if (digitalRead(ROTARY_ENCODER_B_PIN) != B_set) {
		B_set = !B_set;
		//  adjust counter - 1 if B leads A
		if (B_set && !A_set)
			--position;
		//Serial.println(position);
	}
}

bool RotaryEncoder::readAndUpdateIfTimedOut(RotaryEncoder::Result *result) {

	bool timedOut = watchdog->checkTimeOutAndRestart();

	if (timedOut) {
		uint64_t milliSecs = Common::milliSecs();

		storedResult.d_time = milliSecs - time;
		storedResult.d_pos = position;

		/*Serial.print("time: ");
		Serial.print((int)storedResult.d_time);
		Serial.print("\tpos: ");
		Serial.println((int)storedResult.d_pos);*/

		time = milliSecs;
		noInterrupts();
		position = 0;
		interrupts();
	}

	result->d_time = storedResult.d_time;
	result->d_pos = storedResult.d_pos;

	return timedOut;
}

Watchdog *RotaryEncoder::getWatchdog() {
	return watchdog;
}



// Interrupt on A changing state
void doRotaryEncoderA() {
	motorRotaryEncoder->onChange_A();
}

// Interrupt on B changing state, same as A above
void doRotaryEncoderB() {
	motorRotaryEncoder->onChange_B();
}