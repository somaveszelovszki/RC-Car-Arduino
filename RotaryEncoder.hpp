#ifndef ROTARYENCODER_HPP
#define ROTARYENCODER_HPP

#include "Watchdog.hpp"

#define ROTARY_ENCODER_A_PIN 0
#define ROTARY_ENCODER_B_PIN 1

#define ROTARY_RESOLUTION 20

class RotaryEncoder {
public:

	struct Result {
		uint64_t d_time;	// [ms]
		int d_pos;
	};

private:
	uint8_t A_pin;
	uint8_t B_pin;

	bool A_set = false;
	bool B_set = false;

	int position = 0;
	uint64_t time;

	Result storedResult;

	Watchdog *watchdog = new Watchdog(200);	// timeout in ms

public:
	RotaryEncoder(uint8_t A_pin, uint8_t B_pin);

	void initialize();

	void onChange_A();
	void onChange_B();

	/*
		Returns true if timed out, so result is a new valid value
	*/
	bool readAndUpdateIfTimedOut(Result *result);

	Watchdog *getWatchdog();

	void disableInterrupts();
	void enableInterrupts();
};

void doRotaryEncoderA();
void doRotaryEncoderB();

#endif	// ROTARYENCODER_HPP
