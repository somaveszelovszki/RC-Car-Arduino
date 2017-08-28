#ifndef MOTOR_HANDLER_HPP
#define MOTOR_HANDLER_HPP

#include <Servo.h>

#include "Command.hpp"
#include "RotaryEncoder.hpp"

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

		int fwdPin;
		int bwdPin;

		int prevValue = 0;

	public:

		DCMotor(int fwdPin, int bwdPin);

		void writeValue(int value);

		int getPreviousValue();
	};

private:

	double prevError = 0.0;
	double derivative = 0.0;

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

	int getDesiredSpeed();

	//int rotationToSpeed(int rotation, int d_time);
	//int speedToRotation(int speed, int d_time);


public:
// TODO
	MotorHandler(RotaryEncoder *rotaryEncoder);

	void initialize();

	void releaseMotor();

	void updateSpeed();

	bool getActualSpeed(double *speed);


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

	Common::AccelerationDir getDirection();
};

#endif	// MOTOR_HANDLER_HPP