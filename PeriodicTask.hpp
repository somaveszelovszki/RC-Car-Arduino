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
		static PeriodicTask *instances[PT_NUM_TASKS];
		static int numInstances;

		virtual void __initialize() = 0;
		virtual void __onTimedOut() = 0;

	protected:
		Watchdog timeoutCheckWatchdog;

		int getId() const {
			return id;
		}

	public:

		PeriodicTask::PeriodicTask(int _periodTime, int _watchDogTimeout);

		void checkIfTimedOut();

		void run(void *data = NULL);

		static void initializeTasks();
	};
}

#endif	// PERIODIC_TASK_HPP



