#include "DriveController.h"

DriveController::DriveController() {
	motorHandler = new MotorHandler();
	
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
		motorHandler->setSpeed(command.getValueAsInt());
		break;
	case SAFE_DRIVE:

		// if motor has been stopped by the program and the stop timer hasn't finshed, does not let motor run
		if (isStopped) {
			if (stopTimer == 0) {
				isStopped = false;
				motorHandler->setSpeed(command.getValueAsInt());
			}
		} else {
			motorHandler->setSpeed(command.getValueAsInt());
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

	// gets drive mode, direction and speed
	MotorHandler::DCMotor::DIRECTION dir = motorHandler->getDirection();
	int speed_cmpsec = motorHandler->getSpeed_cm_per_sec();

	switch (mode) {
	case FREE_DRIVE:
		return;
	case SAFE_DRIVE:

		// if measured distance is critical, stops the car
		for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
			if (isDistanceCritical((Common::POSITION) sensorPos, distances[sensorPos], speed_cmpsec)) {
				stopDCMotor();
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

bool DriveController::isDistanceCritical(Common::POSITION pos, unsigned long distance, int speed) {

	if (speed == 0) return false;

	static double CRITICAL_PRE_CRASH_TIME = 0.50;	// critical time until crash - in [sec]

	// time until crash with given speed
	double preCrashTime = distance / abs((double)speed);

	if (speed > 0) {		// FORWARD
		if (pos == Common::POSITION::FRONT_LEFT || pos == Common::POSITION::FRONT_RIGHT) {
			/*Serial.print("distance: ");
			Serial.print(distance);
			Serial.print(" cm");
			Serial.print("\t\tspeed: ");
			Serial.println(speed);
			Serial.print(" cm/sec");
			Serial.print(preCrashTime);
			Serial.print(" <= ");
			Serial.println(CRITICAL_PRE_CRASH_TIME);
			Serial.println();*/
			
			// checks if time until crash is below critical
			if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
				Serial.print("\t->\t");
				Serial.println("CRITICAL!!!!!!!!!!!!!!");
				return true;
			}
		}
	} else {		// BACKWARD
		if (pos == Common::POSITION::BACK_LEFT || pos == Common::POSITION::BACK_RIGHT) {

			// checks if time until crash is below critical
			if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
				return true;
			}
		}
	}

	return false;
}


void DriveController::stopDCMotor() {
	motorHandler->setSpeed(DC_COMMAND_VALUE_STOP);
}
