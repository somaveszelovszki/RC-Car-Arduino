#ifndef PERIODIC_THREAD_HPP
#define PERIODIC_THREAD_HPP

#include "Watchdog.hpp"

class PeriodicThread {

private:
	int periodTime;		// period time [ms]
	unsigned long prevTime = 0;		// previous time of execution [ms]

	static PeriodicThread *threads[PT_MAX_NUM_THREADS];
	static int numThreads;

	void onTimedOut() = 0;
	void initialize() = 0;

protected:
	Watchdog *watchdog;

public:

	PeriodicThread::PeriodicThread(int periodTime, int watchDogTimeout) {
		this->periodTime = periodTime;
		this->watchdog = new Watchdog(watchDogTimeout);

		threads[numThreads++] = this;
	}

	bool periodTimeReached() {
		return Common::milliSecs() - prevTime >= static_cast<unsigned long>(periodTime);
	}

	virtual void run() = 0;

	void start() {
		prevTime = Common::milliSecs();
		run();
	}

	bool checkTimedOut() {
		if (watchdog->timedOut())
			onTimedOut();
	}

	static void decrementWatchdogs();

	static void initializeThreads();
};

#endif	// PERIODIC_THREAD_HPP



