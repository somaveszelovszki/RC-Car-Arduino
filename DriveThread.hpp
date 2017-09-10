#ifndef DRIVE_THREAD_HPP
#define DRIVE_THREAD_HPP

#include "MotorHandler.hpp"

class UltrasonicThread;
class CommunicatorThread;
class RotaryThread;

/**
   Controls motors, responsible for driving car.
*/
class DriveThread : public PeriodicThread {

private:
	Common::DriveMode mode;

	MotorHandler motorHandler;

	Watchdog *forceStopWatchdog;

	Command command;

	int distances[ULTRA_NUM_SENSORS];

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

	~DriveThread() {
		delete forceStopWatchdog;
	}
};

#endif	// DRIVE_THREAD_HPP

