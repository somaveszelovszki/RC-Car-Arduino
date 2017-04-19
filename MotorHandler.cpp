#include "MotorHandler.h"

/*
Value is in [-100 +100]
0 means middle position

Position is in [0 +180]
*/
int MotorHandler::commandValueToSteeringAngle(int commandValue) {

	int middle = servoMotor->getMiddlePosition();
	int max = servoMotor->getMaxRotation();

	return map(commandValue, SERVO_COMMAND_VALUE_MIN, SERVO_COMMAND_VALUE_MAX, middle - max, middle + max);
}

void MotorHandler::ServoMotor::recalibrate(int newMiddlePos) {
	MIDDLE_POSITION = newMiddlePos;

	// motor cannot be set to <0 or >MAX_VALUE degrees
	int managableRotation = min(newMiddlePos - SERVO_POSITION_MIN, SERVO_POSITION_MAX - newMiddlePos);

	MAX_ROTATION = min(managableRotation, SERVO_DEFAULT_MAX_ROTATION);
}

int MotorHandler::ServoMotor::getMiddlePosition() {
	return MIDDLE_POSITION;
}

int MotorHandler::ServoMotor::getMaxRotation() {
	return MAX_ROTATION;
}



MotorHandler::DCMotor::DCMotor(unsigned int fwdPin, unsigned int bwdPin) {
	this->fwdPin = fwdPin;
	this->bwdPin = bwdPin;

	pinMode(fwdPin, OUTPUT);
	pinMode(bwdPin, OUTPUT);
}

int MotorHandler::commandValueToSpeed(int commandValue) {
	return map(commandValue, DC_COMMAND_VALUE_MIN, DC_COMMAND_VALUE_MAX, (-1) * MAX_SPEED, MAX_SPEED);
}

void MotorHandler::DCMotor::writeValue(int value) {

	if (value > DC_MAX_VALUE)
		value = DC_MAX_VALUE;

	if (value < DC_MIN_VALUE)
		value = DC_MIN_VALUE;

	analogWrite(fwdPin, value >= 0 ? value : 0);
	analogWrite(bwdPin, value >= 0 ? 0 : (-1 * value));

	int prevValue = value;
}

int MotorHandler::DCMotor::getPreviousValue() {
	return prevValue;
}

void MotorHandler::watchdogDecrement() {
	rotaryEncoder->getWatchdog()->decrement();
}

MotorHandler::DIRECTION MotorHandler::getDirection() {

	int actualSpeed = getActualSpeed();

	return actualSpeed > 0 ? FORWARD : actualSpeed < 0 ? BACKWARD : RELEASE;
}


int MotorHandler::getActualSpeed() {

	RotaryEncoder::Result motorRotation = rotaryEncoder->readAndUpdateIfTimedOut();

	return MOTOR_TRANSFER_RATE * ((motorRotation.d_pos / (double)ROTARY_RESOLUTION) * WHEEL_CIRCUMFERENCE)
		/ motorRotation.d_time * 1000;
}

int MotorHandler::getDesiredSpeed() {
	return desiredSpeed;
}

MotorHandler::MotorHandler(RotaryEncoder *rotaryEncoder) {
	servoMotor = new ServoMotor();
	DC_Motor = new DCMotor(DC_MOTOR_FORWARD_PIN, DC_MOTOR_BACKWARD_PIN);
	this->rotaryEncoder = rotaryEncoder;
	setDesiredSpeed(0);
}


void MotorHandler::initialize() {
	attachServo();
	positionServoMiddle();	releaseMotor();}

void MotorHandler::releaseMotor() {
	setDesiredSpeed(DC_COMMAND_VALUE_STOP);
}

void MotorHandler::updateSpeed() {
	int error = desiredSpeed - getActualSpeed();

	DC_Motor->writeValue(DC_Motor->getPreviousValue() + error * SPEED_CONTROLLER_K);
}

void MotorHandler::positionServoMiddle() {
	servoMotor->write(commandValueToSteeringAngle(SERVO_COMMAND_VALUE_MIDDLE));
}

void MotorHandler::setDesiredSpeed(int commandSpeed) {
	desiredSpeed = commandValueToSpeed(commandSpeed);		// TODO send real command values
}

void MotorHandler::setSteeringAngle(int value) {
	servoMotor->write(value);
}

void MotorHandler::recalibrateServo(int value) {
	servoMotor->recalibrate(value);
}

void MotorHandler::attachServo() {
	servoMotor->attach(SERVO_PIN);
}

void MotorHandler::detachServo() {
	servoMotor->detach();
}