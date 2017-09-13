#ifndef PERIODIC_THREAD_HPP
#define PERIODIC_THREAD_HPP

#include "Watchdog.hpp"
#include "Periodic.hpp"

class PeriodicThread : public Periodic {

private:
	static PeriodicThread *instances[PT_MAX_NUM_THREADS];
	static int numInstances;

	virtual void __initialize() = 0;
	virtual void __run() = 0;
	virtual void __onTimedOut() = 0;

protected:
	Watchdog *watchdog;

public:

	PeriodicThread::PeriodicThread(int _periodTime, int _watchDogTimeout);

	void checkhasTimedOut();

	static void initializeThreads();

	void run();

	~PeriodicThread();
};

#endif	// PERIODIC_THREAD_HPP



