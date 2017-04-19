#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include <Servo.h>
//#include <Adafruit_MotorShield.h>

#include "Command.h"

// servo pin
#define SERVO_PIN 9

#define DC_MOTOR_FORWARD_PIN 5
#define DC_MOTOR_BACKWARD_PIN 11

// servo command value range (0 is servo middle position)
#define SERVO_COMMAND_VALUE_MIN -100
#define SERVO_COMMAND_VALUE_MAX 100
#define SERVO_COMMAND_VALUE_MIDDLE (SERVO_COMMAND_VALUE_MIN + (SERVO_COMMAND_VALUE_MAX - SERVO_COMMAND_VALUE_MIN) / 2)

// servo motor restrictions
#define SERVO_POSITION_MIN 0
#define SERVO_POSITION_MAX 180

// steering construction restriction
#define SERVO_DEFAULT_MAX_ROTATION 70

// DC motor command value range (50 is DC motor stop, >50 means FORWARD)
#define DC_COMMAND_VALUE_MIN 0
#define DC_COMMAND_VALUE_MAX 100
#define DC_COMMAND_VALUE_STOP (DC_COMMAND_VALUE_MIN + (DC_COMMAND_VALUE_MAX - DC_COMMAND_VALUE_MIN) / 2)

// DC Motor value range
#define DC_MAX_VALUE 255

/*
	Handles motors - acceleration and steering.
*/
class MotorHandler {

public:

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

		void writeValue(int value);

	public:

		/*
			Resets servo middle position, checks if MAX_ROTATION hurts any restrictions.
		*/
		void recalibrate(int newMiddlePos);

		/*
			Writes command value to motor.
		*/
		void writeCommandValue(int commandValue);

		/*
			Positions servo motor to middle.
		*/
		void positionMiddle();
	};

	class DCMotor {

	public:
		enum DIRECTION {
			FORWARD, BACKWARD, RELEASE
		};

	private:

		unsigned int fwdPin;
		unsigned int bwdPin;

		int speed;

		void writeValue(int value);
		/*
		Maps command values to actual DC motor speed values.
		*/
		int commandValueToSpeed(int commandValue);

	public:

		DCMotor(unsigned int fwdPin, unsigned int bwdPin);

		void writeCommandValue(int commandValue);

		void release();

		DIRECTION getDirection();
	};

private:

	ServoMotor *servoMotor;
	DCMotor *DC_Motor;


public:
// TODO
	MotorHandler();

	void initialize();

	void setSpeed(int value);
	void setSteeringAngle(int value);
	void recalibrateServo(int value);

	void attachServo();
	void detachServo();

	DCMotor::DIRECTION getDirection();
};

#endif