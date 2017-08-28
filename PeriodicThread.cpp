#include "PeriodicThread.hpp"

int PeriodicThread::numThreads = 0;

void PeriodicThread::decrementWatchdogs() {
	for (int i = 0; i < numThreads; ++i)
		threads[i]->watchdog->decrement();
}

void PeriodicThread::initializeThreads() {
	for (int i = 0; i < numThreads; ++i)
		threads[i]->initialize();
}
