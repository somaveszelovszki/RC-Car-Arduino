#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include <Servo.h>
#include <Adafruit_MotorShield.h>

#include "Command.h"

// motor ports for Adafruit MotorShield
#define MOTOR_1_PORT 1
#define MOTOR_2_PORT 2
#define MOTOR_3_PORT 3
#define MOTOR_4_PORT 4

// DC motor port
#define DC_MOTOR_PORT MOTOR_3_PORT

// servo pin
#define SERVO_PIN 9

// servo command value range (0 is servo middle position)
#define SERVO_MIN_COMMAND_VALUE -100
#define SERVO_MAX_COMMAND_VALUE 100

// servo motor restrictions
#define SERVO_MIN_POSITION 0
#define SERVO_MAX_POSITION 180

// steering construction restriction
#define SERVO_DEFAULT_MAX_ROTATION 70


// DC motor command value range (50 is DC motor stop, >50 means FORWARD)
#define DC_MIN_COMMAND_VALUE 0
#define DC_MAX_COMMAND_VALUE 100

// DC Motor value range
#define DC_MAX_VALUE 255

class MotorHandler {

private:

	/*
		Handles a servo motor.
	*/
	class ServoMotor : public Servo {
	private:
		int MIDDLE_POSITION = 77;	// in degrees
		int MAX_ROTATION = SERVO_DEFAULT_MAX_ROTATION;		// TODO change it to store data

		int currentValue;

		/*
			Maps command values to actual servo values.
		*/
		int commandValueToAngle(int commandValue);

	public:

		/*
			Resets servo middle position, checks if MAX_ROTATION hurts any restrictions.
		*/
		void recalibrate(int newMiddlePos);

		void writeCommandValue(int commandValue);

		void positionMiddle();
	};

	class DCMotor {
	private:

		Adafruit_DCMotor* delegate;

		/*
		Maps command values to actual DC motor speed values.
		*/
		int commandValueToSpeed(int commandValue);

	public:

		DCMotor(Adafruit_DCMotor* DC_Motor);

		void writeCommandValue(int commandValue);

		void release();
	};

	Adafruit_MotorShield* AF_MotorShield;

	DCMotor* DC_Motor;


public:
	ServoMotor* servoMotor;
// TODO
	MotorHandler();

	void initialize();

	void executeCommand_Speed(int value);
	void executeCommand_SteeringAngle(int value);
	void executeCommand_ServoRecalibrate(int value);

};

#endif