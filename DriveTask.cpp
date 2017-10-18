#include "DriveTask.hpp"

using namespace rc_car;

extern CommunicatorTask communicatorTask;
extern UltrasonicTask ultrasonicTask;
extern RotaryTask rotaryTask;

DriveTask::DriveTask() : PeriodicTask(PT_PERIOD_TIME_DRIVE, PT_WATCHDOG_TIMEOUT_DRIVE) {
	forceStopWatchdog = new Watchdog(DRIVE_FORCE_STOP_TIME);

	mode = Common::DriveMode::FREE_DRIVE;		// default mode
}

void DriveTask::__initialize() {
	motorHandler.initialize();
}

void DriveTask::__run() {

	updateValues();

	switch (mode) {
	case Common::FREE_DRIVE:
		break;
	case Common::SAFE_DRIVE:
		// if measured distance is critical, forces car to stop
		/*for (int pos = 0; !forceStopWatchdog->isRunning() && pos < ULTRA_NUM_SENSORS; ++pos)
			if (isDistanceCritical(static_cast<Common::UltrasonicPos>(pos), distances[pos]))
				forceStopWatchdog->restart();*/

		if (msg.getCode() == Message::CODE::Speed && forceStopWatchdog->isRunning())
			msg.setValue(0);

		break;
	case Common::AUTOPILOT:
		break;
	}





	executeMessage();
}

void DriveTask::__onTimedOut() {
	// TODO
}

bool DriveTask::isDistanceCritical(Common::UltrasonicPos pos, int distance) const {

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

void DriveTask::updateValues() {
	ultrasonicTask.getMeasuredPoints(environment.measuredPoints);
	environment.calculate();

	float actualSpeed = rotaryTask.getSpeed();
	motorHandler.updateSpeed(actualSpeed);

	msg = communicatorTask.getMessage();
}

void DriveTask::executeMessage() {
	switch (msg.getCode()) {

	case Message::CODE::Speed:
		motorHandler.setDesiredSpeed(msg.getValueAsFloat());
		break;

	case Message::CODE::SteeringAngle:
		motorHandler.updateSteeringAngle(msg.getValueAsFloat());
		break;

	case Message::CODE::DriveMode:
		mode = static_cast<Common::DriveMode>(msg.getValueAsInt());
		// TODO notification
		break;
	}
}

DriveTask::~DriveTask() {
	delete forceStopWatchdog;
}