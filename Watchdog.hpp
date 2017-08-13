#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include "Common.hpp"

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

#endif	// WATCHDOG_HPP

