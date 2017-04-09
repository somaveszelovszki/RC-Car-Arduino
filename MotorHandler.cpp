#include "MotorHandler.h"

/*
Value is in [-100 +100]
0 means middle position

Position is in [0 +180]
*/
int MotorHandler::ServoMotor::commandValueToAngle(int commandValue) {
	return map(commandValue, SERVO_COMMAND_VALUE_MIN, SERVO_COMMAND_VALUE_MAX, MIDDLE_POSITION - MAX_ROTATION, MIDDLE_POSITION + MAX_ROTATION);
}

void MotorHandler::ServoMotor::recalibrate(int newMiddlePos) {
	MIDDLE_POSITION = newMiddlePos;

	// motor cannot be set to <0 or >MAX_VALUE degrees
	int managableRotation = min(newMiddlePos - SERVO_POSITION_MIN, SERVO_POSITION_MAX - newMiddlePos);

	MAX_ROTATION = min(managableRotation, SERVO_DEFAULT_MAX_ROTATION);

	this->positionMiddle();
}

void MotorHandler::ServoMotor::writeValue(int value) {
	if (this->currentValue != value) {
		this->write(value);

		currentValue = value;
	}
}

void MotorHandler::ServoMotor::writeCommandValue(int commandValue) {

	int newValue = commandValueToAngle(commandValue);

	writeValue(newValue);
}

void MotorHandler::ServoMotor::positionMiddle() {
	this->writeCommandValue(0);
}


MotorHandler::DCMotor::DCMotor(unsigned int fwdPin, unsigned int bwdPin) {
	this->fwdPin = fwdPin;
	this->bwdPin = bwdPin;

	pinMode(fwdPin, OUTPUT);
	pinMode(bwdPin, OUTPUT);
}

int MotorHandler::DCMotor::commandValueToSpeed(int commandValue) {
	return map(commandValue, DC_COMMAND_VALUE_MIN, DC_COMMAND_VALUE_MAX, (-1) * DC_MAX_VALUE, DC_MAX_VALUE);
}

void MotorHandler::DCMotor::writeValue(int value) {

	speed = value;		// TODO read speed from incremental rotary sensor

	analogWrite(fwdPin, value >= 0 ? value : 0);
	analogWrite(bwdPin, value >= 0 ? 0 : (-1 * value));
}

void MotorHandler::DCMotor::writeCommandValue(int commandValue) {

	int speed = commandValueToSpeed(commandValue);

	writeValue(speed);
}

void MotorHandler::DCMotor::release() {
	writeValue(0);
}

MotorHandler::DCMotor::DIRECTION MotorHandler::DCMotor::getDirection() {
	return speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE;
}

int MotorHandler::DCMotor::getSpeed_cm_per_sec() {
	// TODO read speed from incremental rotary sensor
	return speed/2;
}



MotorHandler::MotorHandler() {
	servoMotor = new ServoMotor();
	//AF_MotorShield = new Adafruit_MotorShield();

	DC_Motor = new DCMotor(DC_MOTOR_FORWARD_PIN, DC_MOTOR_BACKWARD_PIN);
}


void MotorHandler::initialize() {
	servoMotor->attach(SERVO_PIN);
	servoMotor->positionMiddle();	DC_Motor->release();}

void MotorHandler::setSpeed(int value) {
	DC_Motor->writeCommandValue(value);
}

void MotorHandler::setSteeringAngle(int value) {
	servoMotor->writeCommandValue(value);
}

void MotorHandler::recalibrateServo(int value) {
	servoMotor->recalibrate(value);
}

MotorHandler::DCMotor::DIRECTION MotorHandler::getDirection() {
	return DC_Motor->getDirection();
}

int MotorHandler::getSpeed_cm_per_sec() {
	return DC_Motor->getSpeed_cm_per_sec();
}


