#ifndef DRIVE_THREAD_HPP
#define DRIVE_THREAD_HPP

#include "MotorHandler.hpp"
#include "Trajectory.hpp"
#include "Environment.hpp"

#include "CommunicatorThread.hpp"
#include "UltrasonicThread.hpp"
#include "RotaryThread.hpp"

/**
   Controls motors, responsible for driving car.
*/
class DriveThread : public PeriodicThread {

private:
	Common::DriveMode mode;

	MotorHandler motorHandler;

	Watchdog *forceStopWatchdog;

	Command cmd;

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

	void executeCommand();

public:
	DriveThread();

	~DriveThread();
};

#endif	// DRIVE_THREAD_HPP

