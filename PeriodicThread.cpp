#include "PeriodicThread.hpp"

int PeriodicThread::numInstances = 0;

void PeriodicThread::checkhasTimedOut() {
	if (watchdog->hasTimedOut())
		__onTimedOut();
}

void PeriodicThread::initializeThreads() {
	for (int i = 0; i < numInstances; ++i)
		instances[i]->__initialize();
}
