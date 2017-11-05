#ifndef PERIODIC_TASK_HPP
#define PERIODIC_TASK_HPP

#include "Watchdog.hpp"
#include "Periodic.hpp"

namespace rc_car {
	/** @brief Base class for task that run periodically.
	*/
	class PeriodicTask : public Periodic {

	private:
		int id;

	protected:
		Watchdog timeoutCheckWatchdog;

		int getId() const {
			return id;
		}

	public:
		PeriodicTask(int _periodTime, int _watchDogTimeout) : Periodic(_periodTime), timeoutCheckWatchdog(_watchDogTimeout) {}

		bool hasTimedOut() {
			return timeoutCheckWatchdog.hasTimedOut();
		}

		void restartTimeoutCheck() {
			timeoutCheckWatchdog.restart();
		}
	};
}

#endif	// PERIODIC_TASK_HPP



