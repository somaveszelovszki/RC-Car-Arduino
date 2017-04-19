#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "Common.h"

/**
   Command class contains data about a received command.
*/
class Watchdog {

private:
	
	unsigned int TIMEOUT;
	unsigned int counter;
	bool running;

public:
	Watchdog(unsigned int timeout);

	void reset();

	void restart();

	void pause();

	void resume();

	bool isRunning();

	void decrement();

	bool timedOut();

	bool checkTimeOutAndRestart();
};

#endif

