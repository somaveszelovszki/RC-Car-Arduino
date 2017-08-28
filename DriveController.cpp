#include "DriveThread.hpp"

DriveThread::DriveThread(RotaryEncoder *rotaryEncoder) : PeriodicThread(PT_PERIOD_TIME_DRIVE) {
	motorHandler = new MotorHandler(rotaryEncoder);
	mode = FREE_DRIVE;		// default mode
}

void DriveThread::initialize() {
	motorHandler->initialize();
}

void DriveThread::run() {

	updateValues();

	switch (command->getCode()) {

	case Command::CODE::Speed:
		executeCommand_Speed(command);
		break;

	case Command::CODE::SteeringAngle:
		executeCommand_SteeringAngle(command);
		break;

	case Command::CODE::ServoRecalibrate:
		executeCommand_ServoRecalibrate(command);
		break;

	case Command::CODE::DriveMode:
		executeCommand_DriveMode(command);
		break;
	}
}

void DriveThread::executeCommand_Speed(const Command *command) {
	switch (mode) {
	case FREE_DRIVE:
		motorHandler->setDesiredSpeed(command->getValueAsInt());
		break;
	case SAFE_DRIVE:

		// if motor has been stopped by the program and the stop timer hasn't finished, does not let motor run
		if (isStopped) {
			if (stopTimer == 0) {
				isStopped = false;
				motorHandler->setDesiredSpeed(command->getValueAsInt());
			}
		} else {
			motorHandler->setDesiredSpeed(command->getValueAsInt());
		}
		break;

	case AUTOPILOT:
		break;
	}
}

void DriveThread::executeCommand_SteeringAngle(const Command *command) {
	motorHandler->setSteeringAngle(command->getValueAsInt());
}

void DriveThread::executeCommand_ServoRecalibrate(const Command *command) {
	motorHandler->recalibrateServo(command->getValueAsInt());
}

void DriveThread::executeCommand_DriveMode(const Command *command) {
	setMode((MODE)command->getValueAsInt());
}


void DriveThread::handleDistanceData(const int distances[ULTRASONIC_NUM_SENSORS]) {

	switch (mode) {
	case FREE_DRIVE:
		return;
	case SAFE_DRIVE:

		// if measured distance is critical, stops the car
		for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
			if (isDistanceCritical((Ultrasonic::POSITION) sensorPos, distances[sensorPos])) {
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

bool DriveThread::isDistanceCritical(Ultrasonic::POSITION pos, int distance) const {

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
		if (pos == Ultrasonic::POSITION::FRONT_LEFT || pos == Ultrasonic::POSITION::FRONT_RIGHT) {
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
		if (pos == Ultrasonic::POSITION::REAR_LEFT || pos == Ultrasonic::POSITION::REAR_RIGHT) {

			// checks if time until crash is below critical
			if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
				return true;
			}
		}
	}

	return false;
}


void DriveThread::releaseMotor() {
	motorHandler->releaseMotor();
}

void DriveThread::attachServoMotor() {
	motorHandler->attachServo();
}

void DriveThread::detachServoMotor() {
	motorHandler->detachServo();
}

void DriveThread::watchdogDecrement() {
	motorHandler->watchdogDecrement();
}

void DriveThread::updateValues() {
	motorHandler->updateSpeed();
}