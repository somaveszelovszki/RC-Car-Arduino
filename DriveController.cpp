#include "DriveController.hpp"

DriveController::DriveController(RotaryEncoder *rotaryEncoder) {
	motorHandler = new MotorHandler(rotaryEncoder);
	
	mode = FREE_DRIVE;		// default mode
}

void DriveController::initialize() {
	motorHandler->initialize();
}

void DriveController::setMode(MODE mode) {
	this->mode = mode;
}

DriveController::MODE DriveController::getMode() {
	return mode;
}


void DriveController::executeCommand_Speed(const Command& command) {
	switch (mode) {
	case FREE_DRIVE:
		motorHandler->setDesiredSpeed(command.getValueAsInt());
		break;
	case SAFE_DRIVE:

		// if motor has been stopped by the program and the stop timer hasn't finished, does not let motor run
		if (isStopped) {
			if (stopTimer == 0) {
				isStopped = false;
				motorHandler->setDesiredSpeed(command.getValueAsInt());
			}
		} else {
			motorHandler->setDesiredSpeed(command.getValueAsInt());
		}
		break;

	case AUTOPILOT:
		break;
	}
}

void DriveController::executeCommand_SteeringAngle(const Command& command) {
	motorHandler->setSteeringAngle(command.getValueAsInt());
}

void DriveController::executeCommand_ServoRecalibrate(const Command& command) {
	motorHandler->recalibrateServo(command.getValueAsInt());
}

void DriveController::executeCommand_DriveMode(const Command& command) {
	setMode((MODE)command.getValueAsInt());
}


void DriveController::handleDistanceData(const unsigned long distances[ULTRASONIC_NUM_SENSORS]) {

	switch (mode) {
	case FREE_DRIVE:
		return;
	case SAFE_DRIVE:

		// if measured distance is critical, stops the car
		for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
			if (isDistanceCritical((SensorHandler::Ultrasonic::POSITION) sensorPos, distances[sensorPos])) {
				releaseMotor();
				isStopped = true;
				stopTimer = EMERGENCY_BREAK_STOP_TIME_MS;
				break;
			}
		}

		break;

	case AUTOPILOT:

		// TODO
		break;
	}
}

bool DriveController::isDistanceCritical(SensorHandler::Ultrasonic::POSITION pos, unsigned long distance) {

	double speed;
	motorHandler->getActualSpeed(&speed);

	//Serial.print("speed: ");
	//Serial.println(speed);

	if (speed == 0) return false;

	// time until crash with given speed
	double preCrashTime = distance / abs(speed);

	Serial.print("pre crash time: ");
	Serial.println(preCrashTime);

	if (speed > 0) {		// FORWARD
		if (pos == SensorHandler::Ultrasonic::POSITION::FRONT_LEFT || pos == SensorHandler::Ultrasonic::POSITION::FRONT_RIGHT) {
			Serial.print("distance: ");
			Serial.print(distance);
			Serial.print(" cm");
			Serial.print("\t\tspeed: ");
			Serial.println(speed);
			Serial.print(" cm/sec");
			Serial.print(preCrashTime);
			Serial.print(" <= ");
			Serial.println(CRITICAL_PRE_CRASH_TIME);
			Serial.println();
			
			// checks if time until crash is below critical
			if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
				Serial.print("\t->\t");
				Serial.println("CRITICAL!");
				return true;
			}
		}
	} else {		// BACKWARD
		if (pos == SensorHandler::Ultrasonic::POSITION::REAR_LEFT || pos == SensorHandler::Ultrasonic::POSITION::REAR_RIGHT) {

			// checks if time until crash is below critical
			if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
				return true;
			}
		}
	}

	return false;
}


void DriveController::releaseMotor() {
	motorHandler->releaseMotor();
}

void DriveController::attachServoMotor() {
	motorHandler->attachServo();
}

void DriveController::detachServoMotor() {
	motorHandler->detachServo();
}

void DriveController::watchdogDecrement() {
	motorHandler->watchdogDecrement();
}

void DriveController::updateValues() {
	motorHandler->updateSpeed();
}