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
	public:
		void writeAngle(float angle);
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
	float steeringAngle;

	ServoMotor servoMotor;
	DCMotor DC_Motor;

public:
	MotorHandler() {
		speedController = new PD_Controller(DC_CONTROL_UPDATE_PERIOD_TIME, DC_CONTROL_Kp, DC_CONTROL_Kd);
	}

	void initialize();

	void updateSpeed(float actualSpeed);

	void MotorHandler::setDesiredSpeed(float newDesiredSpeed) {
		desiredSpeed = newDesiredSpeed;
	}

	void updateSteeringAngle(float _steeringAngle);

	float getSteeringAngle() {
		return steeringAngle;
	}
	/*
	Positions servo motor to middle.
	*/
	void positionServoMiddle();

	void attachServo();
	void detachServo();

	~MotorHandler() {
		delete speedController;
	}
};

#endif	// MOTOR_HANDLER_HPP