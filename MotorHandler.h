#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include <Servo.h>
//#include <Adafruit_MotorShield.h>

#include "Command.h"
#include "RotaryEncoder.h"

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
#define DC_MIN_VALUE (-1) * DC_MAX_VALUE

#define MAX_SPEED 55 // [cm/sec]

#define MOTOR_TRANSFER_RATE 1.0			// transfer rate between motor and wheels (defined by car architecture)
#define WHEEL_CIRCUMFERENCE 20.3		// circumference of motor-powered wheels - in [cm]

#define SPEED_CONTROLLER_K 5


/*
	Handles motors - acceleration and steering.
*/
class MotorHandler {

public:
	enum DIRECTION {
		FORWARD, BACKWARD, RELEASE
	};

	/*
		Handles a servo motor.
	*/
	class ServoMotor : public Servo {
	private:
		int MIDDLE_POSITION = 77;	// in degrees
		int MAX_ROTATION = SERVO_DEFAULT_MAX_ROTATION;		// TODO change it to store data

	public:

		/*
			Resets servo middle position, checks if MAX_ROTATION hurts any restrictions.
		*/
		void recalibrate(int newMiddlePos);

		int getMiddlePosition();
		int getMaxRotation();
	};

	class DCMotor {

	private:

		unsigned int fwdPin;
		unsigned int bwdPin;

		int prevValue = 0;

	public:

		DCMotor(unsigned int fwdPin, unsigned int bwdPin);

		void writeValue(int value);

		int getPreviousValue();
	};

private:

	int desiredSpeed;

	ServoMotor *servoMotor;
	DCMotor *DC_Motor;

	RotaryEncoder *rotaryEncoder;

	/*
	Maps command values to actual DC motor speed values.
	*/
	int commandValueToSpeed(int commandValue);

	/*
	Maps command values to actual servo values.
	*/
	int commandValueToSteeringAngle(int commandValue);

	int getActualSpeed();
	int getDesiredSpeed();


public:
// TODO
	MotorHandler(RotaryEncoder *rotaryEncoder);

	void initialize();

	void releaseMotor();

	void updateSpeed();

	/*
	Positions servo motor to middle.
	*/
	void positionServoMiddle();

	void setDesiredSpeed(int commandSpeed);
	void setSteeringAngle(int commandAngle);
	void recalibrateServo(int commandAngle);

	void attachServo();
	void detachServo();

	void watchdogDecrement();

	DIRECTION getDirection();
};

#endif