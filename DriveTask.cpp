#include "DriveTask.hpp"

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"

using namespace rc_car;

extern CommunicatorTask communicatorTask;
extern UltrasonicTask ultrasonicTask;
extern RotaryTask rotaryTask;

DriveTask::DriveTask() : PeriodicTask(TASK_PERIOD_TIME_DRIVE, TASK_WATCHDOG_TIMEOUT_DRIVE),
    mode(Common::DriveMode::FREE_DRIVE),
    msgWatchdog(DRIVE_MSG_WATCHDOG_TIMEOUT),
    forceSteeringWatchdog(DRIVE_FORCE_STEERING_TIME, Watchdog::State::STOPPED),
    forceStopWatchdog(DRIVE_FORCE_STOP_TIME, Watchdog::State::STOPPED),
    envGridUpdateWatchdog(DRIVE_ENV_GRID_UPDATE_TIMEOUT),
    environment(&car, ultrasonicTask.sensedPoints),
    trajectory(TRAJ_UPDATE_TIMEOUT, &car) {}

void DriveTask::initialize() {
    motorHandler.initialize();
    sendEnvGridEnabled = false;
}

void DriveTask::run() {
    float speed = rotaryTask.getSpeed(), steeringAngle = motorHandler.getSteeringAngle();

    motorHandler.updateSpeed(speed);

    if (trajectory.periodTimeReached()) {
        trajectory.update(speed, steeringAngle);    // updates car properties and trajectory radiuses
        trajectory.restartPeriodCheck();
    }

    if (sendEnvGridEnabled) {
        if (envGridUpdateWatchdog.hasTimedOut()) {
            environment.updateGrid();
            envGridUpdateWatchdog.restart();
        }

        if (!communicatorTask.isSendMsgAvailable(getTaskId())) {
            ByteArray<COMM_MSG_DATA_LENGTH> envGridPart;
            Point2ui8 gridCoords = environment.nextToStream(envGridPart);
            communicatorTask.setMessageToSend(Message(Environment::gridCoordsToCodeByte(gridCoords), envGridPart), getTaskId());
        }
    }

    bool isNewMsgAvailable = communicatorTask.isRecvMsgAvailable(getTaskId());

    if (isNewMsgAvailable)
        msg = communicatorTask.getReceivedMessage(getTaskId());

    switch (mode) {
    case Common::SAFE_DRIVE:
    {
        // calculates section start position according to forward direction
        Common::UltrasonicPos fwdPos = ultrasonicTask.getForwardPos(steeringAngle), sectionCalcStartPos = Common::calcSectionStart(fwdPos);

        // calculates remaining times for sections
        calculateRemainingTimes(sectionCalcStartPos);

        int fwdRelPos = Common::diff(fwdPos, sectionCalcStartPos);

        ////DEBUG_println(remainingTimes[fwdRelPos]);


        bool prev = forceSteeringActive;
        forceSteeringActive |= isDistanceCritical(remainingTimes[fwdRelPos], DRIVE_PRE_CRASH_TIME_DRIVE_CMD_DISABLE);

        if (!prev && forceSteeringActive) {
            //bool newFwdPosFound = false;

            //float newSteeringAngle = ((static_cast<int>(fwdPos) % 6) / 3 ? 1 : -1) * SERVO_MAX_STEERING;
            float newSteeringAngle;
            if (speed > 0) {
                newSteeringAngle = (remainingTimes[Common::diff(Common::UltrasonicPos::FRONT_LEFT_CORNER, sectionCalcStartPos)]
                    > remainingTimes[Common::diff(Common::UltrasonicPos::FRONT_RIGHT_CORNER, sectionCalcStartPos)] ? 1 : -1) * SERVO_MAX_STEERING;
            } else {
                newSteeringAngle = (remainingTimes[Common::diff(Common::UltrasonicPos::REAR_LEFT_CORNER, sectionCalcStartPos)]
                    > remainingTimes[Common::diff(Common::UltrasonicPos::REAR_RIGHT_CORNER, sectionCalcStartPos)] ? 1 : -1) * SERVO_MAX_STEERING;
            }

            motorHandler.updateSteeringAngle(newSteeringAngle);
            forceSteeringWatchdog.restart();

            //for (int i = 0; !newFwdPosFound && i < ENV_SECTIONS_NUM; ++i) {
            //    if (!isDistanceCritical(remainingTimes[i], DRIVE_PRE_CRASH_TIME_DRIVE_CMD_DISABLE)) {
            //        newFwdPosFound = true;
            //        Common::UltrasonicPos newFwdPos = static_cast<Common::UltrasonicPos>(static_cast<int>(globalStartPos) + i);
            //                 //DEBUG_println("newFwdPos: " + String(newFwdPos) + " -> " + ((static_cast<int>(newFwdPos) % 6) / 3 ? 1 : -1));
            //        float newSteeringAngle = SERVO_POS_MID + ((static_cast<int>(newFwdPos) % 6) / 3 ? 1 : -1) * SERVO_ROT_MAX;
            //        motorHandler.updateSteeringAngle(newSteeringAngle);
            //        forceSteeringWatchdog.restart();
            //        forceSteeringActive = true;
            //        ////DEBUG_println("new fwd pos: " + String(newFwdPos) + " -> " + String(ultrasonicTask.getSensorViewAngle(newFwdPos)));
            //        //motorHandler.updateSteeringAngle(ultrasonicTask.getSensorViewAngle(newFwdPos));
            //    }
            //}

            //if (/*!newFwdPosFound && */remainingTimes[fwdRelPos] <= DRIVE_PRE_CRASH_TIME_FORCE_STOP) {
            //    forceStopWatchdog.restart();
            //    forceStopActive = true;
            //}
        }
    }

    // 'break' is missing on purpose!

    case Common::FREE_DRIVE:
        if (isNewMsgAvailable) {
            executeMessage();
            msgWatchdog.restart();
        } else if (msgWatchdog.hasTimedOut()) {
            forceStopWatchdog.restart();
            forceStopActive = true;
        }

        if (forceSteeringActive) {
            if (forceSteeringWatchdog.hasTimedOut()) {
                forceSteeringWatchdog.stop();
                forceSteeringActive = false;
            }
        }

        if (forceStopActive) {
            if (forceStopWatchdog.hasTimedOut()) {
                forceStopWatchdog.stop();
                forceStopActive = false;
            } else
                motorHandler.setDesiredSpeed(0.0f);
        }

        break;
    case Common::AUTOPILOT:
        // TODO
        break;
    }
}

void DriveTask::executeMessage() {
    switch (msg.getCode()) {

    case Message::CODE::Speed:
        if (!forceStopActive)
            motorHandler.setDesiredSpeed(msg.getDataAsFloat());
        break;

    case Message::CODE::SteeringAngle:
        if (!forceSteeringActive)
            motorHandler.updateSteeringAngle(msg.getDataAsFloat());
        break;

    case Message::CODE::DriveMode:
        mode = static_cast<Common::DriveMode>(msg.getDataAsInt());
        communicatorTask.setMessageToSend(Message::ACK, getTaskId());
        break;
    case Message::CODE::EnvGridEn:
        if ((sendEnvGridEnabled = static_cast<bool>(msg.getDataAsInt())))
            envGridUpdateWatchdog.restart();

        communicatorTask.setMessageToSend(Message::ACK, getTaskId());
        break;

    case Message::CODE::ACK_:
    case Message::CODE::RelEnvEn:
    case Message::CODE::RelEnvPoint:
    case Message::CODE::EnvGrid:
    case Message::CODE::CarPos:
    case Message::CODE::CarAngle:
    case Message::CODE::NUM_CODES:
        break;
    }
}

bool DriveTask::isDistanceCritical(float dist, float minDist) {
    return dist == dist       // not NaN
        && dist > 0 && dist <= minDist;
}

void DriveTask::calculateRemainingTimes(Common::UltrasonicPos startPos) {

    const Point2f *pSectionStart, *pSectionEnd;

    ////DEBUG_println("####################");
    ////DEBUG_println("fwdPos: " + String(fwdPos));
    ////DEBUG_println("startPos: " + String(globalStartPos));
    ////DEBUG_println("####################");

    for (int i = 0; i < ENV_SECTIONS_NUM; ++i) {
        pSectionStart = &ultrasonicTask.sensedPoints[startPos];
        pSectionEnd = &ultrasonicTask.sensedPoints[startPos = Common::nextUltrasonicPos(startPos)];
        environment.setSection(pSectionStart, pSectionEnd);

        float minRemainingTime;
        bool obstacle = false;
        ////DEBUG_println("---------------------");
        ////DEBUG_println("section start: " + String(sectionStart.X) + ", " + String(sectionStart.Y));

        while (environment.nextSectionPointExists()) {
            Point2f next = environment.nextSectionPoint();
            ////DEBUG_println("next: " + String(next.X) + ", " + String(next.Y));
            Trajectory::TrackDistance td = trajectory.trackdistancePoint(next);

            if (td.isCritical() && (!obstacle || td.remainingTime < minRemainingTime)) {
                obstacle = true;
                minRemainingTime = td.remainingTime;
            }
        }

        remainingTimes[i] = obstacle ? minRemainingTime : 0.0f;
        ////DEBUG_println(String(i) + ": " + String(remainingTimes[i]));

        pSectionStart = pSectionEnd;
    }
}
