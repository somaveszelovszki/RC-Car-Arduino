#ifndef ROTARY_ENCODER_HPP
#define ROTARY_ENCODER_HPP

#include "Watchdog.hpp"

class RotaryEncoder {
public:

	struct Result {
		unsigned long d_time;	// [ms]
		int d_pos;
	};

private:
	uint8_t A_pin;
	uint8_t B_pin;

	bool A_set = false;
	bool B_set = false;

	int position = 0;
	unsigned long time;

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

#endif	// ROTARY_ENCODER_HPP
