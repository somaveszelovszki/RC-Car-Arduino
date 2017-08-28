#include "MotorHandler.hpp"

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



MotorHandler::DCMotor::DCMotor(int fwdPin, int bwdPin) {
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

	prevValue = value;
}

int MotorHandler::DCMotor::getPreviousValue() {
	return prevValue;
}

void MotorHandler::watchdogDecrement() {
	rotaryEncoder->getWatchdog()->decrement();
}

Common::AccelerationDir MotorHandler::getDirection() {

	double actualSpeed;
	getActualSpeed(&actualSpeed);

	return actualSpeed > 0 ? Common::AccelerationDir::FORWARD
		: actualSpeed < 0 ? Common::AccelerationDir::BACKWARD
		: Common::AccelerationDir::RELEASE;
}

bool MotorHandler::getActualSpeed(double *speed) {

	*speed = (double) desiredSpeed;
	return true;

	/*

	RotaryEncoder::Result rot;
	bool isNewValue = rotaryEncoder->readAndUpdateIfTimedOut(&rot);

	Serial.print("pos: ");
	Serial.print(rot.d_pos);
	Serial.print("time: ");
	Serial.print((int)(rot.d_time));

	*speed = (1 / MOTOR_ROTARY_TRANSFER_RATE) * MOTOR_WHEEL_TRANSFER_RATE
		* ((rot.d_pos / (double) ROTARY_RESOLUTION) * WHEEL_CIRCUMFERENCE) / rot.d_time * 1000;

	Serial.print("speed: ");
	Serial.println(*speed);

	return isNewValue;*/
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
	positionServoMiddle();
	releaseMotor();
	rotaryEncoder->initialize();
}

void MotorHandler::releaseMotor() {
	setDesiredSpeed(DC_COMMAND_VALUE_STOP);
}

void MotorHandler::updateSpeed() {
	double actualSpeed;
	if (getActualSpeed(&actualSpeed)) {
		double error = desiredSpeed - actualSpeed;
		derivative = error - prevError;

		/*Serial.print("desired: ");
		Serial.print(desiredSpeed);
		Serial.print("\tactual: ");
		Serial.println(actualSpeed);
		Serial.print("prev: ");
		Serial.print(DC_Motor->getPreviousValue());
		Serial.print("\twriting: ");
		Serial.println(DC_Motor->getPreviousValue()
			+ error * SPEED_CONTROLLER_Kp
			+ derivative * SPEED_CONTROLLER_Kd);
		Serial.println();*/

		/*DC_Motor->writeValue(DC_Motor->getPreviousValue()
			+ error * SPEED_CONTROLLER_Kp
			+ derivative * SPEED_CONTROLLER_Kd);*/

		DC_Motor->writeValue(desiredSpeed * 5);
	}
}

void MotorHandler::positionServoMiddle() {
	servoMotor->write(commandValueToSteeringAngle(SERVO_COMMAND_VALUE_MIDDLE));
}

void MotorHandler::setDesiredSpeed(int commandSpeed) {
	desiredSpeed = commandValueToSpeed(commandSpeed);		// TODO send real command values
	Serial.print("desired speed: ");
	Serial.println(desiredSpeed);
}

void MotorHandler::setSteeringAngle(int commandAngle) {
	servoMotor->write(commandValueToSteeringAngle(commandAngle));
}

void MotorHandler::recalibrateServo(int commandAngle) {
	servoMotor->recalibrate(commandValueToSteeringAngle(commandAngle));
}

void MotorHandler::attachServo() {
	servoMotor->attach(SERVO_PIN);
}

void MotorHandler::detachServo() {
	servoMotor->detach();
}