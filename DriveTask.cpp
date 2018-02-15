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
    float speed = rotaryTask.getSpeed(),
        steeringAngle = motorHandler.getSteeringAngle();
    motorHandler.updateSpeed(speed);

    bool isNewMsgAvailable = communicatorTask.isRecvMsgAvailable(getTaskId());

    if (isNewMsgAvailable)
        msg = communicatorTask.getReceivedMessage(getTaskId());

    switch (mode) {
    case Common::SAFE_DRIVE:
    {
        //updateEnvironmentGridPoints();

        trajectory.updateValues(speed, steeringAngle);

        // calculates section start position according to forward direction
        Common::UltrasonicPos fwdPos = ultrasonicTask.getForwardPos(steeringAngle),
            sectionCalcStartPos = Common::calcSectionStart(fwdPos);

        // calculates remaining times for sections
        calculateRemainingTimes(sectionCalcStartPos);

        int fwdRelPos = Common::diff(fwdPos, sectionCalcStartPos);

        //.net DEBUG_println(remainingTimes[fwdRelPos]);


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
            //                 DEBUG_println("newFwdPos: " + String(newFwdPos) + " -> " + ((static_cast<int>(newFwdPos) % 6) / 3 ? 1 : -1));
            //        float newSteeringAngle = SERVO_POS_MID + ((static_cast<int>(newFwdPos) % 6) / 3 ? 1 : -1) * SERVO_ROT_MAX;
            //        motorHandler.updateSteeringAngle(newSteeringAngle);
            //        forceSteeringWatchdog.restart();
            //        forceSteeringActive = true;
            //        //DEBUG_println("new fwd pos: " + String(newFwdPos) + " -> " + String(ultrasonicTask.getSensorViewAngle(newFwdPos)));
            //        //motorHandler.updateSteeringAngle(ultrasonicTask.getSensorViewAngle(newFwdPos));
            //    }
            //}

            //if (/*!newFwdPosFound && */remainingTimes[fwdRelPos] <= DRIVE_PRE_CRASH_TIME_FORCE_STOP) {
            //    forceStopWatchdog.restart();
            //    forceStopActive = true;
            //}
        }
    }

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
    }
}

void rc_car::DriveTask::updateEnvironmentGridPoints() {

    const Point2f *sensedPoints[ULTRA_NUM_SENSORS];
    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i)
        sensedPoints[i] = ultrasonicTask.getSensedPoint(static_cast<Common::UltrasonicPos>(i));

    environment.updateGrid(sensedPoints);

    //--------------------------------------------------
    const Point2f *pSectionStart, *pSectionEnd;

    Common::UltrasonicPos sectionStartPos = Common::UltrasonicPos::RIGHT_FRONT;
    pSectionStart = ultrasonicTask.getSensedPoint(sectionStartPos);

    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
        pSectionEnd = ultrasonicTask.getSensedPoint(sectionStartPos = Common::nextUltrasonicPos(sectionStartPos));
        environment.setSection(pSectionStart, pSectionEnd);

        while (environment.nextSectionPointExists()) {
            Point2f sectionPoint = environment.nextSectionPoint();
            environment.setRelativePointObstacle(sectionPoint);
        }

        pSectionStart = pSectionEnd;
    }

    if (!(gridPrintCntr++ % 500) && false)
        environment.print();
}

bool rc_car::DriveTask::isDistanceCritical(float dist, float minDist) {
    return dist == dist       // not NaN
        && dist > 0 && dist <= minDist;
}

void rc_car::DriveTask::calculateRemainingTimes(Common::UltrasonicPos startPos) {

    const Point2f *pSectionStart, *pSectionEnd;

    //DEBUG_println("####################");
    //DEBUG_println("fwdPos: " + String(fwdPos));
    //DEBUG_println("startPos: " + String(globalStartPos));
    //DEBUG_println("####################");

    for (int i = 0; i < ENV_SECTIONS_NUM; ++i) {
        pSectionStart = ultrasonicTask.getSensedPoint(startPos);
        pSectionEnd = ultrasonicTask.getSensedPoint(startPos = Common::nextUltrasonicPos(startPos));
        environment.setSection(pSectionStart, pSectionEnd);

        float minRemainingTime;
        bool obstacle = false;
        //DEBUG_println("---------------------");
        //DEBUG_println("section start: " + String(sectionStart.X) + ", " + String(sectionStart.Y));

        while (environment.nextSectionPointExists()) {
            Point2f next = environment.nextSectionPoint();
            //DEBUG_println("next: " + String(next.X) + ", " + String(next.Y));
            Trajectory::TrackDistance td = trajectory.trackdistancePoint(next);

            if (td.isCritical() && (!obstacle || td.remainingTime < minRemainingTime)) {
                obstacle = true;
                minRemainingTime = td.remainingTime;
            }
        }

        remainingTimes[i] = obstacle ? minRemainingTime : 0.0f;
        //DEBUG_println(String(i) + ": " + String(remainingTimes[i]));

        pSectionStart = pSectionEnd;
    }
}
