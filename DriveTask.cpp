#include "DriveTask.hpp"

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"

using namespace rc_car;

extern CommunicatorTask communicatorTask;
extern UltrasonicTask ultrasonicTask;
extern RotaryTask rotaryTask;

void DriveTask::initialize() {
	motorHandler.initialize();
}

void DriveTask::run() {
	motorHandler.updateSpeed(rotaryTask.getSpeed());
	isNewMsgAvailable = communicatorTask.getReceivedMessage(msg, getId());

	if (Common::testAndSet(&isNewMsgAvailable, false))
		executeMessage();

	// gets track distances for important points (important = measured by sensor in the moving direction)

	Common::UltrasonicPos sectionStartPos, fwdPos = ultrasonicTask.getForwardPos(motorHandler.getSteeringAngle());

	switch (fwdPos) {
	case Common::UltrasonicPos::RIGHT_FRONT:
	case Common::UltrasonicPos::FRONT_RIGHT_CORNER:
		sectionStartPos = Common::UltrasonicPos::RIGHT_FRONT;
		break;
	case Common::UltrasonicPos::FRONT_RIGHT:
	case Common::UltrasonicPos::FRONT_LEFT:
		sectionStartPos = Common::UltrasonicPos::FRONT_RIGHT_CORNER;
		break;
	case Common::UltrasonicPos::FRONT_LEFT_CORNER:
	case Common::UltrasonicPos::LEFT_FRONT:
		sectionStartPos = Common::UltrasonicPos::FRONT_RIGHT;
		break;
	case Common::UltrasonicPos::LEFT_REAR:
	case Common::UltrasonicPos::REAR_LEFT_CORNER:
		sectionStartPos = Common::UltrasonicPos::LEFT_REAR;
		break;
	case Common::UltrasonicPos::REAR_LEFT:
	case Common::UltrasonicPos::REAR_RIGHT:
		sectionStartPos = Common::UltrasonicPos::REAR_LEFT_CORNER;
		break;
	case Common::UltrasonicPos::REAR_RIGHT_CORNER:
	case Common::UltrasonicPos::RIGHT_REAR:
		sectionStartPos = Common::UltrasonicPos::REAR_LEFT;
		break;
	}

	Point<float> sectionStart, sectionEnd;

	for (int i = 0; i < ENV_SECTIONS_NUM; ++i) {
		sectionStart = ultrasonicTask.getPoint(sectionStartPos);
		sectionEnd = ultrasonicTask.getPoint(sectionStartPos = Common::nextUltrasonicPos(sectionStartPos));
		environment.setSection(sectionStart, sectionEnd);

		while (environment.nextSectionPointExists()) {
			Point<float> sectionPoint = environment.calculateNextSectionPoint();
			//
			//
			// TODO do something with section points
			//
			//
		}

		sectionStart = sectionEnd;
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
}

void DriveTask::onTimedOut() {
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
//DEBUG_print("distance: ");
//		DEBUG_print(distance);
//		DEBUG_print(" cm");
//		DEBUG_print("\t\tspeed: ");
//		DEBUG_println(speed);
//		DEBUG_print(" cm/sec");
//		DEBUG_print(preCrashTime);
//		DEBUG_print(" <= ");
//		DEBUG_println(CRITICAL_PRE_CRASH_TIME);
//		DEBUG_println();
//#endif // __DEBUG*/
//
//	//		// checks if time until crash is below critical
//	//		if (preCrashTime <= CRITICAL_PRE_CRASH_TIME) {
//	//			/*#if __DEBUG
//DEBUG_print("\t->\t");
//#endif // __DEBUG
//	//#if __DEBUG
//			DEBUG_println("CRITICAL!");
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