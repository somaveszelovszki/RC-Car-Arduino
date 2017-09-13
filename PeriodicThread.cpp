#include "PeriodicThread.hpp"

int PeriodicThread::numInstances = 0;

PeriodicThread::PeriodicThread(int _periodTime, int _watchDogTimeout) : Periodic(_periodTime) {
	this->watchdog = new Watchdog(_watchDogTimeout);
	instances[numInstances++] = this;
}

void PeriodicThread::checkhasTimedOut() {
	if (watchdog->hasTimedOut())
		__onTimedOut();
}

void PeriodicThread::initializeThreads() {
	for (int i = 0; i < numInstances; ++i)
		instances[i]->__initialize();
}

void PeriodicThread::run() {
	__run();
	prevTime = Common::milliSecs();
}

PeriodicThread::~PeriodicThread() {
	delete watchdog;
}
