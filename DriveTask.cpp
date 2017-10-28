#include "DriveTask.hpp"

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"

using namespace rc_car;

extern CommunicatorTask communicatorTask;
extern UltrasonicTask ultrasonicTask;
extern RotaryTask rotaryTask;

void DriveTask::__initialize() {
	motorHandler.initialize();
}

void DriveTask::__run(void *unused) {
	updateValues();

	if (Common::testAndSet(&isNewMsgAvailable, false)) {
		// TODO handle new message from communicator
#if __DEBUG
		//Common::debug_println(msg.toString());
		//Common::debug_println("new message arrived.");
#endif // __DEBUG
	}

	switch (mode) {
	case Common::FREE_DRIVE:
		break;
	case Common::SAFE_DRIVE:
		// if measured distance is critical, forces car to stop
		/*for (int pos = 0; !forceStopWatchdog->isRunning() && pos < ULTRA_NUM_SENSORS; ++pos)
		if (isDistanceCritical(static_cast<Common::UltrasonicPos>(pos), distances[pos])) {
		forceStopWatchdog.restart();
		forceStopActive = true;
		}*/

		if (forceStopActive && msg.getCode() == Message::CODE::Speed) {
			if (forceStopWatchdog.hasTimedOut())
				forceStopActive = false;
			else
				msg.setData(static_cast<int32_t>(0));
		}

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
//
//	float speed;
//	//motorHandler.getActualSpeed(&speed);
//
//	// time until crash with given speed
//	float preCrashTime = distance / abs(speed);
//
//	if (speed > 0) {		// FORWARD
//	//	if (pos == Common::UltrasonicPos::FRONT_LEFT || pos == Common::UltrasonicPos::FRONT_RIGHT) {
//#if __DEBUG
//Common::debug_print("distance: ");
//		Common::debug_print(distance);
//		Common::debug_print(" cm");
//		Common::debug_print("\t\tspeed: ");
//		Common::debug_println(speed);
//		Common::debug_print(" cm/sec");
//		Common::debug_print(preCrashTime);
//		Common::debug_print(" <= ");
//		Common::debug_println(CRITICAL_PRE_CRASH_TIME);
//		Common::debug_println();
//#endif // __DEBUG*/
//
//	//		// checks if time until crash is below critical
//	//		if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
//	//			/*#if __DEBUG
//Common::debug_print("\t->\t");
//#endif // __DEBUG
//	//#if __DEBUG
//			Common::debug_println("CRITICAL!");
//#endif // __DEBUG*/
//	//			return true;
//	//		}
//	//	}
//	//} else {		// BACKWARD
//	//	if (pos == Common::UltrasonicPos::REAR_LEFT || pos == Common::UltrasonicPos::REAR_RIGHT) {
//
//	//		// checks if time until crash is below critical
//	//		if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
//	//			return true;
//	//		}
//	//	}
//	////}
//
	return false;
}

void DriveTask::updateValues() {
	float actualSpeed = rotaryTask.getSpeed();
	motorHandler.updateSpeed(actualSpeed);

	isNewMsgAvailable = communicatorTask.getReceivedMessage(msg, getId());
	if (isNewMsgAvailable) {
#if __DEBUG
		communicatorTask.setMessageToSend(Message(Message::ACK), getId());
#endif // __DEBUG);
	}
}

void DriveTask::executeMessage() {
	switch (msg.getCode()) {

	case Message::CODE::Speed:
		motorHandler.setDesiredSpeed(msg.getDataAsFloat());
		break;

	case Message::CODE::SteeringAngle:
		motorHandler.updateSteeringAngle(msg.getDataAsFloat());
		break;

	case Message::CODE::DriveMode:
		mode = static_cast<Common::DriveMode>(msg.getDataAsInt());
		// TODO notification
		break;
	}
}