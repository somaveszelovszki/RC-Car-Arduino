#ifndef RC_CAR__MOTOR_HANDLER__HPP
#define RC_CAR__MOTOR_HANDLER__HPP

//#include <Servo.h>
#include <ServoTimer1.h>
#include "Message.hpp"
#include "PD_Controller.hpp"

namespace rc_car {
	/** @brief Abstract layer for handling DC motor and servomotor.
	*/
	class MotorHandler {

	private:

		/*
			Handles a servo motor.
		*/
		class ServoMotor : public ServoTimer1 {
			friend class MotorHandler;
		public:
			void writeAngle(float _angle);
		};

		class DCMotor {
			friend class MotorHandler;

		private:
			int prevValue = 0;

		public:
			DCMotor() {}
			void initialize();
			void writeValue(int _value);
		};

		PD_Controller speedCtrl;

		float desiredSpeed;
		float steeringAngle;

		ServoMotor servoMotor;
		DCMotor DC_Motor;

	public:
		MotorHandler() : speedCtrl(DC_CONTROL_UPDATE_PERIOD_TIME, DC_CONTROL_Kp, DC_CONTROL_Kd) {}

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
	};
}

#endif // RC_CAR__MOTOR_HANDLER__HPP
