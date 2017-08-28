#include "RotaryEncoder.hpp"

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
	attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), doRotaryEncoderA, CHANGE);
	// encoder pin on interrupt 1 (pin 3)
	//attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_B_PIN), doRotaryEncoderB, CHANGE);

	watchdog->restart();

	position = 0;
	time = 0;
	storedResult.d_time = 0;
	storedResult.d_pos = 0;
}

void RotaryEncoder::onChange_A() {
	A_set = digitalRead(ROTARY_ENCODER_A_PIN);
	if (A_set == digitalRead(ROTARY_ENCODER_A_PIN)) {

		// adjust counter + 1 if A leads B
		if (A_set && !B_set)
			++position;
		Serial.println("A");
	}
}

void RotaryEncoder::onChange_B() {
	B_set = digitalRead(ROTARY_ENCODER_B_PIN);
	if (B_set == digitalRead(ROTARY_ENCODER_B_PIN)) {

		//  adjust counter - 1 if B leads A
		if (B_set && !A_set)
			--position;
		//Serial.println(position);
		Serial.println("B");
	}
}

bool RotaryEncoder::readAndUpdateIfTimedOut(RotaryEncoder::Result *result) {

	bool timedOut = watchdog->checkTimeOutAndRestart();

	if (timedOut) {
		unsigned long milliSecs = Common::milliSecs();

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

void RotaryEncoder::disableInterrupts() {
	detachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN));
}

void RotaryEncoder::enableInterrupts() {
	attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), doRotaryEncoderA, CHANGE);
}



// Interrupt on A changing state
void doRotaryEncoderA() {
	//motorRotaryEncoder->onChange_A();
}

// Interrupt on B changing state, same as A above
void doRotaryEncoderB() {
	//motorRotaryEncoder->onChange_B();
}