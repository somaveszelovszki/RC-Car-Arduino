#ifndef DRIVECONTROLLER_H
#define DRIVECONTROLLER_H

#include "MotorHandler.h"
#include "SensorHandler.h"

#define EMERGENCY_BREAK_STOP_TIME_MS 3000;

/**
   Controls motors, responsible for driving car.
*/
class DriveController {

public:

	/*
		Drive mode defines if the program should override user's commands.
	*/
	enum MODE {
		FREE_DRIVE = 1,		// user has total control, no override
		SAFE_DRIVE = 2,		// user has limited control, crash avoidance
		AUTOPILOT = 3		// user has no control, program drives
	};

private:
	MODE mode;
	MotorHandler* motorHandler;

	// needed when program in SAFE_DRIVE mode stops car
	bool isStopped = false;

public:

	DriveController();

	/*
		Initializes motors.
	*/
	void initialize();

	/*
		Sets drive mode.
	*/
	void setMode(MODE mode);

	/*
		Gets drive mode.
	*/
	MODE getMode();


	// executor methods

	void executeCommand_Speed(const Command& command);
	void executeCommand_SteeringAngle(const Command& command);
	void executeCommand_ServoRecalibrate(const Command& command);
	void executeCommand_DriveMode(const Command& command);


	// handler methods

	/*
		Handles measured distances - drive mode defines action.
	*/
	void handleDistanceData(const unsigned long distances[ULTRASONIC_NUM_SENSORS]);

	/*
		Decides if given distance at given position and at given speed is critical - need to stop the car.
		TODO replace with DANGER_LEVELs
	*/
	bool isDistanceCritical(Common::POSITION pos, unsigned long distance, int speed);

	/*
		Stops DC motor.
	*/
	void stopDCMotor();



	// TODO replace with a better solution for restarting motor
	unsigned int stopTimer;
};

#endif

