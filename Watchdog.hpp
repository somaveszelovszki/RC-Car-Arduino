#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include "Common.hpp"

/**
   Command class contains data about a received command.
*/
class Watchdog {

public:
	enum State {
		RESET, RUNNING, PAUSED, TIMED_OUT
	};

private:
	int TIMEOUT;
	int counter;
	State state;

	static Watchdog instances[WD_MAX_NUM_WATCHDOGS];
	static int numInstances;

public:
	Watchdog(int _timeout);

	void reset();

	void restart();

	void pause() {
		state = PAUSED;
	}

	void resume() {
		state = RUNNING;
	}

	bool isRunning() const {
		return state == RUNNING;
	}

	bool hasTimedOut() const {
		return state == TIMED_OUT;
	}

	void decrement();

	static void decrementAll();
};

#endif	// WATCHDOG_HPP

