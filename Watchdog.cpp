#include "Watchdog.hpp"

int Watchdog::numInstances = 0;

Watchdog::Watchdog(int _timeout) {
	TIMEOUT = _timeout;
	reset();

	instances[numInstances++] = this;
}

void Watchdog::reset() {
	noInterrupts();
	counter = TIMEOUT;
	state = RESET;
	interrupts();
}

void Watchdog::restart() {
	noInterrupts();
	counter = TIMEOUT;
	state = RUNNING;
	interrupts();
}

void Watchdog::decrement() {
	if (state == RUNNING && counter > 0 && --counter == 0)
		state = TIMED_OUT;
}

void Watchdog::decrementAll() {
	for (int i = 0; i < numInstances; ++i)
		instances[i]->decrement();
}
