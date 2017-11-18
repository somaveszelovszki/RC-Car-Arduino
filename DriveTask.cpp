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
	float speed = rotaryTask.getSpeed(), steeringAngle = motorHandler.getSteeringAngle();
	motorHandler.updateSpeed(speed);

	CommunicatorTask::MsgAvailability msgAvailability = communicatorTask.getRecvAvailability(getTaskId());

	if (msgAvailability)
		communicatorTask.getReceivedMessage(msg, getTaskId());

	switch (mode) {
	case Common::SAFE_DRIVE:
	{
		updateEnvironmentGridPoints();

		trajectory.updateValues(speed, steeringAngle);

		float remainingTimes[ENV_SECTIONS_NUM];

		Point2f sectionStart, sectionEnd;

		// calculates section start position according to forward direction
		Common::UltrasonicPos fwdPos = ultrasonicTask.getForwardPos(steeringAngle),
			globalStartPos = Common::calculateSectionStartPos(fwdPos),
			sectionStartPos = globalStartPos;

		for (int i = 0; i < ENV_SECTIONS_NUM; ++i) {
			sectionStart = ultrasonicTask.getSensedPoint(sectionStartPos);
			sectionEnd = ultrasonicTask.getSensedPoint(sectionStartPos = Common::nextUltrasonicPos(sectionStartPos));
			environment.setSection(sectionStart, sectionEnd);

			float minRemainingTime;
			bool obstacle = false;

			while (environment.nextSectionPointExists()) {
				Trajectory::TrackDistance td = trajectory.trackDistanceFromPoint(environment.nextSectionPoint());

				if (td.isCritical() && !obstacle || td.remainingTime < minRemainingTime) {
					obstacle = true;
					minRemainingTime = td.remainingTime;
				}
			}

			remainingTimes[i] = obstacle ? minRemainingTime : 0.0f;
			//DEBUG_println(String(i) + ": " + String(remainingTimes[i]));

			sectionStart = sectionEnd;
		}

		/*int fwdRelPos = static_cast<int>(fwdPos) - static_cast<int>(globalStartPos);

		driveCmdEnabled = !remainingTimes[fwdRelPos] || remainingTimes[fwdRelPos] > DRIVE_PRE_CRASH_TIME_DRIVE_CMD_DISABLE;

		if (!driveCmdEnabled) {
			bool newFwdPosFound = false;

			for (int i = 0; !newFwdPosFound && i < ENV_SECTIONS_NUM; ++i) {
				if (!remainingTimes[i] || remainingTimes[i] > DRIVE_PRE_CRASH_TIME_DRIVE_CMD_DISABLE) {
					newFwdPosFound = true;
					Common::UltrasonicPos newFwdPos = static_cast<Common::UltrasonicPos>(static_cast<int>(globalStartPos) + i);
					motorHandler.updateSteeringAngle(ultrasonicTask.getSensorViewAngle(newFwdPos));
				}
			}

			if (!newFwdPosFound && remainingTimes[fwdRelPos] <= DRIVE_PRE_CRASH_TIME_FORCE_STOP) {
				forceStopWatchdog.restart();
				forceStopActive = true;
			}
		}*/
	}

	case Common::FREE_DRIVE:
		if (msgAvailability) {
			executeMessage();
			msgWatchdog.restart();
		} else if (msgWatchdog.hasTimedOut()) {
			forceStopWatchdog.restart();
			forceStopActive = true;
		}

		if (forceStopActive) {
			if (forceStopWatchdog.hasTimedOut()) {
				forceStopWatchdog.stop();
				forceStopActive = false;
				driveCmdEnabled = true;
			} else
				motorHandler.setDesiredSpeed(0.0f);
		}

		break;
	case Common::AUTOPILOT:
		// TODO
		break;
	}
}

void DriveTask::onTimedOut() {
	restartTimeoutCheck();
}

void DriveTask::executeMessage() {
	switch (msg.getCode()) {

	case Message::CODE::Speed:
		if (driveCmdEnabled)
			motorHandler.setDesiredSpeed(msg.getDataAsFloat());
		break;

	case Message::CODE::SteeringAngle:
		if (driveCmdEnabled)
			motorHandler.updateSteeringAngle(msg.getDataAsFloat());
		break;

	case Message::CODE::DriveMode:
		mode = static_cast<Common::DriveMode>(msg.getDataAsInt());
		communicatorTask.setMessageToSend(Message::ACK, getTaskId());
		break;
	}
}

void rc_car::DriveTask::updateEnvironmentGridPoints() {
	Point2f sectionStart, sectionEnd;

	Common::UltrasonicPos sectionStartPos = Common::UltrasonicPos::RIGHT_FRONT;

	for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
		sectionEnd = ultrasonicTask.getSensedPoint(sectionStartPos = Common::nextUltrasonicPos(sectionStartPos));
		environment.setSection(sectionStart, sectionEnd);

		while (environment.nextSectionPointExists()) {
			Point2f sectionPoint = environment.nextSectionPoint();
			environment.setRelativePointObstacle(sectionPoint);
		}

		sectionStart = sectionEnd;
	}

	if (!(gridPrintCntr++ % 500) && false)
		environment.print();
}
