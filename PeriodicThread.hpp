#ifndef PERIODICTHREAD_HPP
#define PERIODICTHREAD_HPP

#include "Common.hpp"

class PeriodicThread {

private:

	unsigned long cyclePeriod;

public:

	PeriodicThread(unsigned long cyclePeriod);

	bool periodCycleThresholdReached(unsigned long cycleCounter);
};

#endif	// PERIODICTHREAD_HPP



