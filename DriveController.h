#ifndef DRIVECONTROLLER_H
#define DRIVECONTROLLER_H

#include "MotorHandler.h"
#include "SensorHandler.h"

/**
   Controls motors, responsible for driving car.
*/
class DriveController {

public:
	enum MODE {
		FREE_DRIVE = 1,
		SAFE_DRIVE = 2,
		AUTOPILOT = 3
	};

private:
	MODE mode;

	MotorHandler* motorHandler;
	SensorHandler* sensorHandler;


public:

	DriveController();

	void initialize();

	void setModeAndInit(MODE mode);

	MODE getMode();

	void executeCommand_Speed(Command* command);

	void executeCommand_SteeringAngle(Command* command);

	void executeCommand_ServoRecalibrate(Command* command);
};

#endif

