#include "DriveThread.hpp"

extern CommunicatorThread communicatorThread;
extern UltrasonicThread ultrasonicThread;
extern RotaryThread rotaryThread;

DriveThread::DriveThread() : PeriodicThread(PT_PERIOD_TIME_DRIVE, PT_WATCHDOG_TIMEOUT_DRIVE) {
	forceStopWatchdog = new Watchdog(DRIVE_FORCE_STOP_TIME);

	mode = Common::DriveMode::FREE_DRIVE;		// default mode
}

void DriveThread::__initialize() {
	motorHandler.initialize();
}

void DriveThread::__run() {

	updateValues();

	switch (mode) {
	case Common::FREE_DRIVE:
		break;
	case Common::SAFE_DRIVE:
		// if measured distance is critical, forces car to stop
		/*for (int pos = 0; !forceStopWatchdog->isRunning() && pos < ULTRA_NUM_SENSORS; ++pos)
			if (isDistanceCritical(static_cast<Common::UltrasonicPos>(pos), distances[pos]))
				forceStopWatchdog->restart();*/

		if (cmd.getCode() == Command::CODE::Speed && forceStopWatchdog->isRunning())
			cmd.setIntValue(0);

		break;
	case Common::AUTOPILOT:
		break;
	}





	executeCommand();
}

void DriveThread::__onTimedOut() {
	// TODO
}

bool DriveThread::isDistanceCritical(Common::UltrasonicPos pos, int distance) const {

	//float speed;
	//motorHandler.getActualSpeed(&speed);

	////Serial.print("speed: ");
	////Serial.println(speed);

	//if (speed == 0) return false;

	//// time until crash with given speed
	//float preCrashTime = distance / abs(speed);

	//Serial.print("pre crash time: ");
	//Serial.println(preCrashTime);

	//if (speed > 0) {		// FORWARD
	//	if (pos == Common::UltrasonicPos::FRONT_LEFT || pos == Common::UltrasonicPos::FRONT_RIGHT) {
	//		/*Serial.print("distance: ");
	//		Serial.print(distance);
	//		Serial.print(" cm");
	//		Serial.print("\t\tspeed: ");
	//		Serial.println(speed);
	//		Serial.print(" cm/sec");
	//		Serial.print(preCrashTime);
	//		Serial.print(" <= ");
	//		Serial.println(CRITICAL_PRE_CRASH_TIME);
	//		Serial.println();*/

	//		// checks if time until crash is below critical
	//		if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
	//			/*Serial.print("\t->\t");
	//			Serial.println("CRITICAL!");*/
	//			return true;
	//		}
	//	}
	//} else {		// BACKWARD
	//	if (pos == Common::UltrasonicPos::REAR_LEFT || pos == Common::UltrasonicPos::REAR_RIGHT) {

	//		// checks if time until crash is below critical
	//		if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
	//			return true;
	//		}
	//	}
	//}

	return false;
}

void DriveThread::updateValues() {
	ultrasonicThread.getSensedPoints(environment.points);
	environment.fwdDir = ultrasonicThread.calculateForwardDirection(motorHandler.getSteeringAngle());
	environment.calculateCombinedSections();

	float actualSpeed = rotaryThread.getSpeed();
	motorHandler.updateSpeed(actualSpeed);

	cmd = communicatorThread.getCommand();
}

void DriveThread::executeCommand() {
	switch (cmd.getCode()) {

	case Command::CODE::Speed:
		motorHandler.setDesiredSpeed(cmd.getFloatValue());
		break;

	case Command::CODE::SteeringAngle:
		motorHandler.updateSteeringAngle(cmd.getFloatValue());
		break;

	case Command::CODE::DriveMode:
		mode = cmd.getDriveModeValue();
		// TODO notification
		break;
	}
}

DriveThread::~DriveThread() {
	delete forceStopWatchdog;
}