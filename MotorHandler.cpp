#include "MotorHandler.hpp"

/*
Value is in [-100 +100]
0 means middle position

Position is in [0 +180]
*/
int MotorHandler::commandValueToSteeringAngle(int commandValue) const {
	return map(commandValue, SERVO_COMMAND_VALUE_MIN, SERVO_COMMAND_VALUE_MAX,
		servoMotor.middlePos - servoMotor.maxRotation, servoMotor.middlePos + servoMotor.maxRotation);
}

void MotorHandler::ServoMotor::recalibrate(int newMiddlePos) {
	middlePos = newMiddlePos;

	// motor cannot be set to <0 or >MAX_VALUE degrees
	int managableRotation = min(newMiddlePos - SERVO_POSITION_MIN, SERVO_POSITION_MAX - newMiddlePos);

	maxRotation = min(managableRotation, SERVO_DEFAULT_MAX_ROTATION);
}

void MotorHandler::DCMotor::initialize() {
	pinMode(DC_MOTOR_FORWARD_PIN, OUTPUT);
	pinMode(DC_MOTOR_BACKWARD_PIN, OUTPUT);
}

void MotorHandler::DCMotor::writeValue(int value) {

	if (value > DC_MAX_VALUE)
		value = DC_MAX_VALUE;

	if (value < DC_MIN_VALUE)
		value = DC_MIN_VALUE;

	analogWrite(DC_MOTOR_FORWARD_PIN, value >= 0 ? value : 0);
	analogWrite(DC_MOTOR_BACKWARD_PIN, value >= 0 ? 0 : (-1 * value));
}

void MotorHandler::initialize() {
	DC_Motor.initialize();
	attachServo();
	positionServoMiddle();
	setDesiredSpeed(0.0f);
}

void MotorHandler::updateSpeed(float actualSpeed) {

	// if controller's period time has been reached, speed has to be updated
	// -> controller's 'run' method is called with the error (difference between desired and actual speed)
	if (speedController->periodTimeReached())
		DC_Motor.writeValue(speedController->run(desiredSpeed - actualSpeed));
}

void MotorHandler::setSteeringAngle(int commandAngle) {
	servoMotor.write(commandValueToSteeringAngle(commandAngle));
}

void MotorHandler::positionServoMiddle() {
	setSteeringAngle(0);
}

void MotorHandler::recalibrateServo(int commandAngle) {
	servoMotor.recalibrate(commandValueToSteeringAngle(commandAngle));
}

void MotorHandler::attachServo() {
	servoMotor.attach(SERVO_PIN);
}

void MotorHandler::detachServo() {
	servoMotor.detach();
}