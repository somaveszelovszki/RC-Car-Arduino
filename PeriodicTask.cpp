#include "PeriodicTask.hpp"

using namespace rc_car;

PeriodicTask *PeriodicTask::instances[PT_NUM_TASKS];
int PeriodicTask::numInstances = 0;

PeriodicTask::PeriodicTask(int _periodTime, int _watchDogTimeout) : Periodic(_periodTime), timeoutCheckWatchdog(_watchDogTimeout) {
	instances[id = numInstances++] = this;
}

void PeriodicTask::checkIfTimedOut() {
	if (timeoutCheckWatchdog.hasTimedOut())
		__onTimedOut();
}

void PeriodicTask::initializeTasks() {
	for (int i = 0; i < numInstances; ++i)
		instances[i]->__initialize();
}

void PeriodicTask::run(void *data = NULL) {
	Periodic::run();
	timeoutCheckWatchdog.restart();
}
