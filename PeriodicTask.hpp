#ifndef PERIODIC_TASK_HPP
#define PERIODIC_TASK_HPP

#include "Watchdog.hpp"
#include "Periodic.hpp"

namespace rc_car {
	/** @brief Base class for task that run periodically.
	*/
	class PeriodicTask : public Periodic {

	private:
		static PeriodicTask *instances[PT_MAX_NUM_TASKS];
		static int numInstances;

		virtual void __initialize() = 0;
		virtual void __run() = 0;
		virtual void __onTimedOut() = 0;

	protected:
		Watchdog *watchdog;

	public:

		PeriodicTask::PeriodicTask(int _periodTime, int _watchDogTimeout);

		void checkIfTimedOut();

		static void initializeTasks();

		void run();

		~PeriodicTask();
	};
}

#endif	// PERIODIC_TASK_HPP



