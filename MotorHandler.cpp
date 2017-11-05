#include "MotorHandler.hpp"

using namespace rc_car;

void MotorHandler::ServoMotor::writeAngle(float angle) {
	int _value = static_cast<int>(angle + M_PI_2 * RAD_TO_DEG);
	if (_value >= SERVO_POS_MIN && _value <= SERVO_POS_MAX)
		write(_value);
}

void MotorHandler::DCMotor::initialize() {
	pinMode(DC_MOTOR_FORWARD_PIN, OUTPUT);
	pinMode(DC_MOTOR_BACKWARD_PIN, OUTPUT);
}

void MotorHandler::DCMotor::writeValue(float value) {

	int _value = static_cast<int>(value);

	if (_value >= DC_MIN_VALUE && _value <= DC_MAX_VALUE) {
		bool isFwd = _value >= 0;

		analogWrite(DC_MOTOR_FORWARD_PIN, isFwd ? _value : 0);
		analogWrite(DC_MOTOR_BACKWARD_PIN, isFwd ? 0 : -_value);
	}
}

void MotorHandler::initialize() {
	DC_Motor.initialize();
	attachServo();
	positionServoMiddle();
	setDesiredSpeed(0.0f);
	speedCtrl.restartPeriodCheck();
}

void MotorHandler::updateSpeed(float actualSpeed) {

	// if controller's period time has been reached, speed has to be updated
	// -> controller's 'run' method is called with the error (difference between desired and actual speed)
	if (speedCtrl.periodTimeReached()) {
		float error = desiredSpeed - actualSpeed;
		speedCtrl.run(error);
		DC_Motor.writeValue(speedCtrl.getOutput());
	}
	//DC_Motor.writeValue(desiredSpeed * 4);	// TODO
}

void MotorHandler::updateSteeringAngle(float _steeringAngle) {
	steeringAngle = _steeringAngle;
	servoMotor.writeAngle(steeringAngle);
}

void MotorHandler::positionServoMiddle() {
	updateSteeringAngle(0.0f);
}

void MotorHandler::attachServo() {
	servoMotor.attach(SERVO_PIN);
}

void MotorHandler::detachServo() {
	servoMotor.detach();
}