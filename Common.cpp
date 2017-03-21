#include "Common.h"

size_t Common::write(Print& printer, const String& str) {

	size_t numPrintedBytes = 0;

	// prints all characters
	for (size_t i = 0; i < str.length(); i++) {
		numPrintedBytes += printer.print(str.charAt(i));
	}

	return numPrintedBytes;
}

void Common::initTimer() {
	noInterrupts();

	// initializes Timer3 - interrupt in every 1 ms

	/*
	
	// sets compare match register for 1kHz increments
	OCR3A = 249;	// = (16*10^6) / (1000*64) - 1 (must be <256)

	TCCR3A = 0;     // set entire TCCR1A register to 0
	TCCR3B = 0;     // same for TCCR1B

	// sets CS21 and CS20 bits for 64 prescaler
	TCCR3B |= (1 << CS31) | (1 << CS30);

	// turns on CTC mode (Clear Timer on Compare Match)
	TCCR3B |= (1 << WGM32);

	// enables timer compare interrupt
	TIMSK3 |= _BV(OCIE3A);

	*/

	// Timer0 is already used for millis() - we'll just interrupt somewhere
	// in the middle and call the "Compare A" function below
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);

	interrupts();
}

template <typename T>
static void Common::arrayCopy(T dest[], T src[], size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dest[i] = src[i];
	}
}

