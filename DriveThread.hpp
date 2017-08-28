#ifndef DRIVE_THREAD_HPP
#define DRIVE_THREAD_HPP

#include "MotorHandler.hpp"
#include "SensorThread.hpp"

/**
   Controls motors, responsible for driving car.
*/
class DriveThread : public PeriodicThread {

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

	const Command *command;

	void onTimedOut();

	/*
	Initializes motors.
	*/
	void initialize();

public:

	int distances[ULTRASONIC_NUM_SENSORS];

	DriveThread(RotaryEncoder *rotaryEncoder);

	void run();

	/*
		Sets drive mode.
	*/
	void setMode(MODE mode) {
		this->mode = mode;
	}

	void start(const Command *command) {
		this->command = command;
		PeriodicThread::start();
	}

	/*
		Gets drive mode.
	*/
	MODE getMode() const {
		return mode;
	}


	// executor methods

	void executeCommand_Speed(const Command *command);
	void executeCommand_SteeringAngle(const Command *command);
	void executeCommand_ServoRecalibrate(const Command *command);
	void executeCommand_DriveMode(const Command *command);


	// handler methods

	/*
		Handles measured distances - drive mode defines action.
	*/
	void handleDistanceData(const int distances[ULTRASONIC_NUM_SENSORS]);

	/*
		Decides if given distance at given position and at given speed is critical - need to stop the car.
		TODO replace with DANGER_LEVELs
	*/
	bool isDistanceCritical(Ultrasonic::POSITION pos, int distance) const;

	/*
		Stops DC motor.
	*/
	void releaseMotor();

	void attachServoMotor();
	void detachServoMotor();

	void watchdogDecrement();

	void updateValues();

	// TODO replace with a better solution for restarting motor
	int stopTimer;

};

#endif	// DRIVE_THREAD_HPP

