#ifndef MOTOR_HANDLER_HPP
#define MOTOR_HANDLER_HPP

#include <Servo.h>

#include "Command.hpp"
#include "PD_Controller.hpp"

/*
	Handles motors - acceleration and steering.
*/
class MotorHandler {

private:

	/*
		Handles a servo motor.
	*/
	class ServoMotor : public Servo {
		friend class MotorHandler;

	private:
		int middlePos = 77;	// in degrees
		int maxRotation = SERVO_DEFAULT_MAX_ROTATION;		// TODO change it to store data

	public:
		ServoMotor() {}
		/*
			Resets servo middle position, checks if MAX_ROTATION hurts any restrictions.
		*/
		void recalibrate(int newMiddlePos);
	};

	class DCMotor {
		friend class MotorHandler;

	private:
		int prevValue = 0;

	public:
		DCMotor() {}
		void initialize();
		void writeValue(int value);
	};

	PD_Controller *speedController;

	float desiredSpeed;

	ServoMotor servoMotor;
	DCMotor DC_Motor;

	/*
	Maps command values to actual servo values.
	*/
	int commandValueToSteeringAngle(int commandValue) const;

	//int rotationToSpeed(int rotation, int d_time);
	//int speedToRotation(int speed, int d_time);


public:
	MotorHandler() {
		speedController = new PD_Controller(DC_CONTROL_UPDATE_PERIOD_TIME, DC_CONTROL_Kp, DC_CONTROL_Kd);
	}

	void initialize();

	void updateSpeed(float actualSpeed);

	void MotorHandler::setDesiredSpeed(float newDesiredSpeed) {
		desiredSpeed = newDesiredSpeed;
	}

	void setSteeringAngle(int commandAngle);
	/*
	Positions servo motor to middle.
	*/
	void positionServoMiddle();
	void recalibrateServo(int commandAngle);

	void attachServo();
	void detachServo();

	~MotorHandler() {
		delete speedController;
	}
};

#endif	// MOTOR_HANDLER_HPP