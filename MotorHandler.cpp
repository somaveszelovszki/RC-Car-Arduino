#include "MotorHandler.h"

/*
Value is in [-100 +100]
0 means middle position

Position is in [0 +180]
*/
int MotorHandler::ServoMotor::commandValueToAngle(int commandValue) {
	return map(commandValue, SERVO_MIN_COMMAND_VALUE, SERVO_MAX_COMMAND_VALUE, MIDDLE_POSITION - MAX_ROTATION, MIDDLE_POSITION + MAX_ROTATION);
}

void MotorHandler::ServoMotor::recalibrate(int newMiddlePos) {
	MIDDLE_POSITION = newMiddlePos;

	int managableRotation = min(newMiddlePos - SERVO_MIN_POSITION, SERVO_MAX_POSITION - newMiddlePos);	// motor cannot be set to <0 or >MAX_VALUE degrees

	MAX_ROTATION = min(managableRotation, SERVO_DEFAULT_MAX_ROTATION);

	this->positionMiddle();
}

void MotorHandler::ServoMotor::writeCommandValue(int commandValue) {

	int newValue = commandValueToAngle(commandValue);

	if (this->currentValue != newValue) {
		this->write(newValue);

		currentValue = newValue;
	}
}

void MotorHandler::ServoMotor::positionMiddle() {
	this->writeCommandValue(0);
}

MotorHandler::DCMotor::DCMotor(Adafruit_DCMotor* DC_Motor) {
	this->delegate = DC_Motor;
}


int MotorHandler::DCMotor::commandValueToSpeed(int commandValue) {
	return map(commandValue, DC_MIN_COMMAND_VALUE, DC_MAX_COMMAND_VALUE, (-1) * DC_MAX_VALUE, DC_MAX_VALUE);
}

void MotorHandler::DCMotor::writeCommandValue(int commandValue) {

	int speed = commandValueToSpeed(commandValue);

	delegate->setSpeed(abs(speed));
	delegate->run(speed > 0 ? FORWARD : speed < 0 ? BACKWARD : RELEASE);
}

void MotorHandler::DCMotor::release() {
	delegate->run(RELEASE);
}

MotorHandler::MotorHandler() {
	servoMotor = new ServoMotor();
	AF_MotorShield = new Adafruit_MotorShield();
	DC_Motor = new DCMotor(AF_MotorShield->getMotor(DC_MOTOR_PORT));
}


void MotorHandler::initialize() {
	servoMotor->attach(SERVO_PIN);

	servoMotor->positionMiddle();
	AF_MotorShield->begin();
}

void MotorHandler::executeCommand_Speed(int value) {
	DC_Motor->writeCommandValue(value);
}

void MotorHandler::executeCommand_SteeringAngle(int value) {
	servoMotor->writeCommandValue(value);
}

void MotorHandler::executeCommand_ServoRecalibrate(int value) {
	servoMotor->recalibrate(value);
}


