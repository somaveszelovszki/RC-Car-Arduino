#include "Watchdog.hpp"

Watchdog::Watchdog(int timeout) {
	TIMEOUT = timeout;
	running = false;
}

void Watchdog::reset() {
	counter = TIMEOUT;
	pause();
}

void Watchdog::restart() {
	noInterrupts();
	reset();
	resume();
	interrupts();
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
	bool result = false;
	noInterrupts();
	if (counter == 0) {
		reset();
		result = true;
	}
	interrupts();
	return result;
}

bool Watchdog::checkTimeOutAndRestart() {

	if (timedOut()) {
		restart();
		return true;
	}
	return false;
}
