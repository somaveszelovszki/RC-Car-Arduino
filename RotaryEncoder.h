#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

#include "Watchdog.h"

#define ROTARY_ENCODER_A_PIN 2
#define ROTARY_ENCODER_B_PIN 3

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

	int position;
	uint64_t time;

	Result storedResult;

	Watchdog *watchdog = new Watchdog(200);	// timeout in ms

public:
	RotaryEncoder(uint8_t A_pin, uint8_t B_pin);

	void initialize();

	void onChange_A();
	void onChange_B();

	Result readAndUpdateIfTimedOut();

	Watchdog *getWatchdog();
};

void doRotaryEncoderA();
void doRotaryEncoderB();

#endif
