#include "DriveController.h"

DriveController::DriveController() {
	motorHandler = new MotorHandler();
	sensorHandler = new SensorHandler();
}

void DriveController::initialize() {
	motorHandler->initialize();
}

void DriveController::setModeAndInit(MODE mode) {
	this->mode = mode;

	// TODO reinit
}

DriveController::MODE DriveController::getMode() {
	return mode;
}

void DriveController::executeCommand_Speed(Command* command) {
	motorHandler->executeCommand_Speed(command->getValueAsInt());
}

void DriveController::executeCommand_SteeringAngle(Command* command) {
	motorHandler->executeCommand_SteeringAngle(command->getValueAsInt());
}

void DriveController::executeCommand_ServoRecalibrate(Command* command) {
	motorHandler->executeCommand_ServoRecalibrate(command->getValueAsInt());
}