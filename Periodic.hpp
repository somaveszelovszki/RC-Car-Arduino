#ifndef RC_CAR__PERIODIC__HPP
#define RC_CAR__PERIODIC__HPP

#include "Watchdog.hpp"

namespace rc_car {
	/** @brief Base class for all objects that need to run periodically.
	*/
	class Periodic {

	private:
		Watchdog periodWatchdog;

	public:
		Periodic(int _periodTime) : periodWatchdog(_periodTime) {}

		void restartPeriodCheck() {
			periodWatchdog.restart();
		}

		bool periodTimeReached() const {
			return periodWatchdog.hasTimedOut();
		}

		unsigned long getPrevCalledTime() const {
			return periodWatchdog.getStartTime();
		}
	};
}

#endif // RC_CAR__PERIODIC__HPP

