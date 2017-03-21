#include "PeriodicThread.h"

PeriodicThread::PeriodicThread(unsigned long cyclePeriod) {

	Serial.print("cyclePeriod set to ");
	Serial.println(cyclePeriod);

	this->cyclePeriod = cyclePeriod;
}

bool PeriodicThread::periodCycleThresholdReached(unsigned long cycleCounter) {
	return cycleCounter % cyclePeriod == 0;
}