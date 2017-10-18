#ifndef DRIVE_TASK_HPP
#define DRIVE_TASK_HPP

#include "MotorHandler.hpp"
#include "Trajectory.hpp"
#include "Environment.hpp"

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"

namespace rc_car {
	/** @brief Makes driving decisions from sensor data and car state. Controls motors via the MotorHandler.
	*/
	class DriveTask : public PeriodicTask {

	private:
		Common::DriveMode mode;

		MotorHandler motorHandler;

		Watchdog *forceStopWatchdog;

		Message msg;

		Environment environment;

		void __initialize() override;
		void __run() override;
		void __onTimedOut() override;

		/*
			Decides if given distance at given position and at given speed is critical - need to stop the car.
			TODO replace with DANGER_LEVELs
		*/
		bool isDistanceCritical(Common::UltrasonicPos pos, int distance) const;

		void updateValues();

		void executeMessage();

	public:
		DriveTask();

		~DriveTask();
	};

}
#endif	// DRIVE_TASK_HPP

