#ifndef RC_CAR__MOTOR_HANDLER__HPP
#define RC_CAR__MOTOR_HANDLER__HPP

#include <ServoTimer1.h>
#include "Message.hpp"
#include "PD_Controller.hpp"

namespace rc_car {
	/** @brief Abstract layer for handling DC motor and servomotor.
	*/
	class MotorHandler {

	private:

		/** @brief Handles a servo motor.
		*/
		class ServoMotor : public ServoTimer1 {
			friend class MotorHandler;
		public:
            /** @brief Writes value to servo motor in order to achieve given steering angle.

            @param _angle The desired steering angle in [degree].
            */
			void writeAngle(float _angle);
		};

        /** @brief Handles a DC motor.
        */
		class DCMotor {
		public:
            /** @brief Sets pin mode for motor control pins.
            */
			void initialize();

            /** @brief Writes PWM speed value on the control pins.

            @param _value The PWM speed value to set.
            */
			void writeValue(int _value);
		};

        /** @brief The speed controller.
        */
		PD_Controller speedCtrl;

        /** @brief The current desired speed.
        This is the speed that can be set from the outside, the actual PWM values will be calculated by the speed controller.
        */
		float desiredSpeed;

        /** @brief The current steering angle.
        */
		float steeringAngle;

        /** @brief Controls the servo motor.
        */
		ServoMotor servoMotor;

        /** @brief Controls the DC motor.
        */
		DCMotor DC_Motor;

	public:
        /** @brief Constructor - initializes the speed controller.
        */
		MotorHandler() : speedCtrl(DC_CONTROL_UPDATE_PERIOD_TIME, DC_CONTROL_Kp, DC_CONTROL_Kd,
            static_cast<float>(DC_MIN_value), static_cast<float>(DC_MAX_value)) {}

        /** @brief Initializes the motors and sets them to there start position (positions servo to middle position and stops DC motor).
        */
		void initialize();

        /** @brief Updates the DC motor's PWM value according to the actual measured speed using the speed controller.

        @param actualSpeed The current measured speed (acquired from the RotaryTask).
        */
		void updateSpeed(float actualSpeed);

        /** @brief Sets desired speed.

        @param newDesiredSpeed The desired speed to set.
        */
		void MotorHandler::setDesiredSpeed(float newDesiredSpeed) {
			desiredSpeed = newDesiredSpeed;
		}

        /** @brief Sets steering angle and writes it to the servo motor.

        @param _steeringAngle The steering angle to set.
        */
        void updateSteeringAngle(float _steeringAngle) {
            servoMotor.writeAngle(steeringAngle = _steeringAngle);
        }

        /** @brief Gets the current steering angle.

        @returns The current steering angle.
        */
		float getSteeringAngle() {
			return steeringAngle;
		}
		/** @brief Positions servo motor to middle position.
		*/
        void MotorHandler::positionServoMiddle() {
            updateSteeringAngle(0.0f);
        }

        /** @brief Attaches servo motor to the configured pin.
        */
        void MotorHandler::attachServo() {
            servoMotor.attach(SERVO_PIN);
        }

        /** @brief Detaches servo motor.
        */
        void MotorHandler::detachServo() {
            servoMotor.detach();
        }
	};
}

#endif // RC_CAR__MOTOR_HANDLER__HPP
