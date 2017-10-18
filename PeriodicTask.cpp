#include "PeriodicTask.hpp"

using namespace rc_car;

PeriodicTask *PeriodicTask::instances[PT_MAX_NUM_TASKS];
int PeriodicTask::numInstances = 0;

PeriodicTask::PeriodicTask(int _periodTime, int _watchDogTimeout) : Periodic(_periodTime) {
	this->watchdog = new Watchdog(_watchDogTimeout);
	instances[numInstances++] = this;
}

void PeriodicTask::checkIfTimedOut() {
	if (watchdog->hasTimedOut())
		__onTimedOut();
}

void PeriodicTask::initializeTasks() {
	for (int i = 0; i < numInstances; ++i)
		instances[i]->__initialize();
}

void PeriodicTask::run() {
	__run();
	prevTime = Common::milliSecs();
}

PeriodicTask::~PeriodicTask() {
	delete watchdog;
}
