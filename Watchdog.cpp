#include "Watchdog.h"

Watchdog::Watchdog(unsigned int timeout) {
	TIMEOUT = timeout;
	running = false;
}

void Watchdog::reset() {
	counter = TIMEOUT;
	pause();
}

void Watchdog::restart() {
	reset();
	resume();
}

void Watchdog::pause() {
	running = false;
}

void Watchdog::resume() {
	running = true;
}

bool Watchdog::isRunning() {
	return running;
}

void Watchdog::decrement() {
	if (isRunning()) {
		counter = counter > 0 ? counter - 1 : 0;
	}
	
}

bool Watchdog::timedOut() {
	if (counter == 0) {
		reset();
		return true;
	}
	return false;
}
