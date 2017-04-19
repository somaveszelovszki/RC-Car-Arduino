#ifndef DRIVECONTROLLER_H
#define DRIVECONTROLLER_H

#include "MotorHandler.h"
#include "SensorHandler.h"

#define EMERGENCY_BREAK_STOP_TIME_MS 3000;

#define MOTOR_TRANSFER_RATE 1.0			// transfer rate between motor and wheels (defined by car architecture)
#define WHEEL_CIRCUMFERENCE 20.3		// circumference of motor-powered wheels - in [cm]
#define CRITICAL_PRE_CRASH_TIME 0.5		// critical time until crash - in [sec]

#define MAX_SPEED 55 // [cm/sec]

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
	MotorHandler *motorHandler;

	// needed when program in SAFE_DRIVE mode stops car
	bool isStopped = false;

	int speed = 0;		// wheel speed in cm/sec

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

	void handleMotorRotationData(SensorHandler::RotaryEncoder::Result motorRotation);

	/*
		Decides if given distance at given position and at given speed is critical - need to stop the car.
		TODO replace with DANGER_LEVELs
	*/
	bool isDistanceCritical(Common::POSITION pos, unsigned long distance);

	/*
		Stops DC motor.
	*/
	void stopDCMotor();

	void attachServoMotor();
	void detachServoMotor();

	// TODO replace with a better solution for restarting motor
	unsigned int stopTimer;
};

#endif

