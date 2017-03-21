#ifndef PERIODICTHREAD
#define PERIODICTHREAD

#include "Common.h"

class PeriodicThread {

private:

	unsigned long cyclePeriod;

public:

	PeriodicThread(unsigned long cyclePeriod);

	bool periodCycleThresholdReached(unsigned long cycleCounter);
};

#endif



