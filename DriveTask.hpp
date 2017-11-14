#ifndef RC_CAR__DRIVE_TASK__HPP
#define RC_CAR__DRIVE_TASK__HPP

#include "MotorHandler.hpp"
#include "Trajectory.hpp"
#include "Environment.hpp"

namespace rc_car {
	/** @brief Makes driving decisions from sensor data and car state. Controls motors via the MotorHandler.
	*/
	class DriveTask : public PeriodicTask {

	private:
		Common::DriveMode mode;
		MotorHandler motorHandler;
		Watchdog forceStopWatchdog;
		Environment environment;
		bool isNewMsgAvailable = false;
		Message msg;

		bool forceStopActive = false;

		/*
			Decides if given distance at given position and at given speed is critical - need to stop the car.
			TODO replace with DANGER_LEVELs
		*/
		bool isDistanceCritical(Common::UltrasonicPos pos, int distance) const;

		void executeMessage();
	public:
		DriveTask() : PeriodicTask(PT_PERIOD_TIME_DRIVE, PT_WATCHDOG_TIMEOUT_DRIVE),
			forceStopWatchdog(DRIVE_FORCE_STOP_TIME),

			// TODO remove this line, initialize mode from phone
			mode(Common::DriveMode::FREE_DRIVE) {}

		void initialize();
		void run();
		void onTimedOut();
	};

}
#endif // RC_CAR__DRIVE_TASK__HPP

