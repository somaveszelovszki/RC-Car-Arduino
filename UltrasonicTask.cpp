#include "UltrasonicTask.hpp"

#include "CommunicatorTask.hpp"

using namespace rc_car;

extern UltrasonicTask ultrasonicTask;
extern CommunicatorTask communicatorTask;

const Common::Validation UltrasonicTask::MAX_DIST_VALIDATION = {
    ULTRA_VALID_MAX_DIST_SAMPLE_NUM, ULTRA_VALID_MAX_DIST_REL_ERR
};

const Common::Validation UltrasonicTask::DEF_VALIDATION = {
    ULTRA_VALID_DEF_SAMPLE_NUM, ULTRA_VALID_DEF_REL_ERR
};

UltrasonicTask::UltrasonicTask() : PeriodicTask(TASK_PERIOD_TIME_ULTRASONIC, TASK_WATCHDOG_TIMEOUT_ULTRASONIC),
sensorConnection(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, ULTRA_MAX_DIST),
echoWatchdog(ULTRA_ECHO_TIMEOUT) {

    setEnabled(false);

    sensors[Common::UltrasonicPos::RIGHT_FRONT].pos.X = ULTRA_POS_X_RF;
    sensors[Common::UltrasonicPos::RIGHT_FRONT].pos.Y = ULTRA_POS_Y_RF;
    sensors[Common::UltrasonicPos::RIGHT_FRONT].viewAngle = ULTRA_VIEW_ANGLE_RF;
    sensors[Common::UltrasonicPos::RIGHT_FRONT].selIdx = ULTRA_IDX_RF;



    sensors[Common::UltrasonicPos::FRONT_RIGHT_CORNER].pos.X = ULTRA_POS_X_FRC;
    sensors[Common::UltrasonicPos::FRONT_RIGHT_CORNER].pos.Y = ULTRA_POS_Y_FRC;
    sensors[Common::UltrasonicPos::FRONT_RIGHT_CORNER].viewAngle = ULTRA_VIEW_ANGLE_FRC;
    sensors[Common::UltrasonicPos::FRONT_RIGHT_CORNER].selIdx = ULTRA_IDX_FRC;

    sensors[Common::UltrasonicPos::FRONT_RIGHT].pos.X = ULTRA_POS_X_FR;
    sensors[Common::UltrasonicPos::FRONT_RIGHT].pos.Y = ULTRA_POS_Y_FR;
    sensors[Common::UltrasonicPos::FRONT_RIGHT].viewAngle = ULTRA_VIEW_ANGLE_FR;
    sensors[Common::UltrasonicPos::FRONT_RIGHT].selIdx = ULTRA_IDX_FR;

    sensors[Common::UltrasonicPos::FRONT_LEFT].pos.X = ULTRA_POS_X_FL;
    sensors[Common::UltrasonicPos::FRONT_LEFT].pos.Y = ULTRA_POS_Y_FL;
    sensors[Common::UltrasonicPos::FRONT_LEFT].viewAngle = ULTRA_VIEW_ANGLE_FL;
    sensors[Common::UltrasonicPos::FRONT_LEFT].selIdx = ULTRA_IDX_FL;

    sensors[Common::UltrasonicPos::FRONT_LEFT_CORNER].pos.X = ULTRA_POS_X_FLC;
    sensors[Common::UltrasonicPos::FRONT_LEFT_CORNER].pos.Y = ULTRA_POS_Y_FLC;
    sensors[Common::UltrasonicPos::FRONT_LEFT_CORNER].viewAngle = ULTRA_VIEW_ANGLE_FLC;
    sensors[Common::UltrasonicPos::FRONT_LEFT_CORNER].selIdx = ULTRA_IDX_FLC;



    sensors[Common::UltrasonicPos::LEFT_FRONT].pos.X = ULTRA_POS_X_LF;
    sensors[Common::UltrasonicPos::LEFT_FRONT].pos.Y = ULTRA_POS_Y_LF;
    sensors[Common::UltrasonicPos::LEFT_FRONT].viewAngle = ULTRA_VIEW_ANGLE_LF;
    sensors[Common::UltrasonicPos::LEFT_FRONT].selIdx = ULTRA_IDX_LF;

    sensors[Common::UltrasonicPos::LEFT_REAR].pos.X = ULTRA_POS_X_LR;
    sensors[Common::UltrasonicPos::LEFT_REAR].pos.Y = ULTRA_POS_Y_LR;
    sensors[Common::UltrasonicPos::LEFT_REAR].viewAngle = ULTRA_VIEW_ANGLE_LR;
    sensors[Common::UltrasonicPos::LEFT_REAR].selIdx = ULTRA_IDX_LR;



    sensors[Common::UltrasonicPos::REAR_LEFT_CORNER].pos.X = ULTRA_POS_X_RLC;
    sensors[Common::UltrasonicPos::REAR_LEFT_CORNER].pos.Y = ULTRA_POS_Y_RLC;
    sensors[Common::UltrasonicPos::REAR_LEFT_CORNER].viewAngle = ULTRA_VIEW_ANGLE_RLC;
    sensors[Common::UltrasonicPos::REAR_LEFT_CORNER].selIdx = ULTRA_IDX_RLC;

    sensors[Common::UltrasonicPos::REAR_LEFT].pos.X = ULTRA_POS_X_RL;
    sensors[Common::UltrasonicPos::REAR_LEFT].pos.Y = ULTRA_POS_Y_RL;
    sensors[Common::UltrasonicPos::REAR_LEFT].viewAngle = ULTRA_VIEW_ANGLE_RL;
    sensors[Common::UltrasonicPos::REAR_LEFT].selIdx = ULTRA_IDX_RL;

    sensors[Common::UltrasonicPos::REAR_RIGHT].pos.X = ULTRA_POS_X_RR;
    sensors[Common::UltrasonicPos::REAR_RIGHT].pos.Y = ULTRA_POS_Y_RR;
    sensors[Common::UltrasonicPos::REAR_RIGHT].viewAngle = ULTRA_VIEW_ANGLE_RR;
    sensors[Common::UltrasonicPos::REAR_RIGHT].selIdx = ULTRA_IDX_RR;

    sensors[Common::UltrasonicPos::REAR_RIGHT_CORNER].pos.X = ULTRA_POS_X_RRC;
    sensors[Common::UltrasonicPos::REAR_RIGHT_CORNER].pos.Y = ULTRA_POS_Y_RRC;
    sensors[Common::UltrasonicPos::REAR_RIGHT_CORNER].viewAngle = ULTRA_VIEW_ANGLE_RRC;
    sensors[Common::UltrasonicPos::REAR_RIGHT_CORNER].selIdx = ULTRA_IDX_RRC;



    sensors[Common::UltrasonicPos::RIGHT_REAR].pos.X = ULTRA_POS_X_RiR;
    sensors[Common::UltrasonicPos::RIGHT_REAR].pos.Y = ULTRA_POS_Y_RiR;
    sensors[Common::UltrasonicPos::RIGHT_REAR].viewAngle = ULTRA_VIEW_ANGLE_RiR;
    sensors[Common::UltrasonicPos::RIGHT_REAR].selIdx = ULTRA_IDX_RiR;
}

void UltrasonicTask::initialize() {
    pinMode(ULTRA_SEL_0_PIN, OUTPUT);
    pinMode(ULTRA_SEL_1_PIN, OUTPUT);
    pinMode(ULTRA_SEL_2_PIN, OUTPUT);
    pinMode(ULTRA_SEL_3_PIN, OUTPUT);

    // initializes sensor data - measured, stored and validatied distances, nonResponsiveCounter
    for (int pos = 0; pos < ULTRA_NUM_SENSORS; ++pos) {
        sensors[pos].dist_measured = sensors[pos].dist_valid = ULTRA_MAX_DIST;

        for (int sampleIndex = 0; sampleIndex < ULTRA_NUM_DIST_SAMPLES; ++sampleIndex)
            sensors[pos].dist_stored[sampleIndex] = ULTRA_MAX_DIST;
    }

    setEnabled(true);
    busy = false;
    currentSensorPos = static_cast<Common::UltrasonicPos>(ULTRA_NUM_SENSORS - 1);
    currentSampleIndex = ULTRA_NUM_DIST_SAMPLES - 1;

    sendRelEnvEnabled = false;
}

void UltrasonicTask::run() {

    if (isEnabled()) {
        if (communicatorTask.isRecvMsgAvailable(getTaskId())) {
            msg = communicatorTask.getReceivedMessage(getTaskId());
            executeMessage();
        }

        if (!isBusy()) {

            if (sendRelEnvEnabled && !currentRelEnvPointSent && currentSensorPos % 2) {
                msg.setCode(ultraPosToMsgCode(currentSensorPos));

                ByteArray<2> lowBytes, highBytes;
                sensedPoints[static_cast<int>(currentSensorPos) - 1].toByteArray(lowBytes);
                sensedPoints[currentSensorPos].toByteArray(highBytes);
                msg.setData(lowBytes + highBytes);

                communicatorTask.setMessageToSend(msg, getTaskId());

                currentRelEnvPointSent = true;
            }

            pingNextSensor();
        }
    }
}

void UltrasonicTask::pingNextSensor() {
    // if cycle finished, increments sample index
    if (measurementCycleFinished())
        currentSampleIndex = (currentSampleIndex + 1) % ULTRA_NUM_DIST_SAMPLES;

    // validates previously measured point
    sensors[currentSensorPos].validate(currentSampleIndex);
    updatePoint(static_cast<Common::UltrasonicPos>(currentSensorPos));

    // increments sensor position
    currentSensorPos = Common::nextUltrasonicPos(currentSensorPos);
    currentRelEnvPointSent = false;

    updateSensorSelection();
    busy = true;
    sensorConnection.timer_stop();
    echoWatchdog.restart();
    sensorConnection.ping_timer(ultrasonicEchoCheckIT);
}

void UltrasonicTask::echoCheck() {
    if (sensorConnection.check_timer()) {
        sensors[currentSensorPos].dist_measured = sensorConnection.ping_result / US_ROUNDTRIP_CM;
        busy = false;

    } else if (echoWatchdog.hasTimedOut()) {
        sensors[currentSensorPos].dist_measured = ULTRA_MAX_DIST;
        busy = false;
        sensorConnection.timer_stop();
    }
}

void UltrasonicTask::updateSensorSelection() {

    uint8_t pos = static_cast<uint8_t>(sensors[currentSensorPos].selIdx);

    // writes position to selection wires (as binary value)
    digitalWrite(ULTRA_SEL_0_PIN, (pos & 0x01) ? HIGH : LOW);
    digitalWrite(ULTRA_SEL_1_PIN, (pos & 0x02) ? HIGH : LOW);
    digitalWrite(ULTRA_SEL_2_PIN, (pos & 0x04) ? HIGH : LOW);
    digitalWrite(ULTRA_SEL_3_PIN, (pos & 0x08) ? HIGH : LOW);
}

void UltrasonicTask::Sensor::validate(int sampleIndex) {
    // sensor response is 0 if (measured distance > ULTRA_MAX_DIST)
    dist_stored[sampleIndex] = dist_measured ? dist_measured : ULTRA_MAX_DIST;

    Common::Validation validation = dist_measured == ULTRA_MAX_DIST ?
        UltrasonicTask::MAX_DIST_VALIDATION : UltrasonicTask::DEF_VALIDATION;

    int prevValidatedValue = dist_valid;
    int currentStoredValue = dist_stored[sampleIndex];

    /*
    Checks if current stored value is in the given range of the previous validated value for the sensor.
    e.g. if the previous validated value was 150, we will believe 145 without hesitation, no need for further validation
    */
    if (Common::isInRange(currentStoredValue, prevValidatedValue, validation.relErr)) {
        dist_valid = currentStoredValue;
    } else {
        // if we got here, current stored value is out of range of the previous validated value so we need to validate

        int sampleCount = 0;
        bool outOfRangeFound = false;

        // iterates through stored values (from latest to earliest) and counts how many values are in the given range of this value
        // latest (minSampleNum - 1) results should be in the range for us to believe that current value is valid

        for (int idx = (sampleIndex + ULTRA_NUM_DIST_SAMPLES - 1) % ULTRA_NUM_DIST_SAMPLES;
            !outOfRangeFound && (idx != sampleIndex) && sampleCount < validation.minSampleNum;
            idx = (idx + ULTRA_NUM_DIST_SAMPLES - 1) % ULTRA_NUM_DIST_SAMPLES) {

            if (Common::isInRange(dist_stored[idx], dist_measured, validation.relErr))
                ++sampleCount;
            else
                outOfRangeFound = true;
        }

        // validation OK -> we believe that distance is really what we measured
        // validation FAILED -> validated value will be previous validated value
        dist_valid = sampleCount >= validation.minSampleNum - 1 ? currentStoredValue : prevValidatedValue;
    }
}

void UltrasonicTask::updatePoint(Common::UltrasonicPos sensorPos) {
    // view angles are relative to the y-axis, but now we need the angle frm the x-axis
    // the conversion is defined by dirAngleToXY(viewAngle)
    Sensor& sensor = sensors[sensorPos];
    sensedPoints[sensorPos].X = sensor.pos.X + sensor.dist_valid * cos(dirAngleToXY(sensor.viewAngle));
    sensedPoints[sensorPos].Y = sensor.pos.Y + sensor.dist_valid * sin(dirAngleToXY(sensor.viewAngle));
}

Common::UltrasonicPos UltrasonicTask::getForwardPos(float steeringAngle) const {
    float _min = abs(steeringAngle - sensors[0].viewAngle * RAD_TO_DEG);
    int minPos = 0;

    for (int pos = 1; pos < ULTRA_NUM_SENSORS; ++pos) {
        float current = abs(steeringAngle - sensors[pos].viewAngle * RAD_TO_DEG);
        if (current < _min) {
            _min = current;
            minPos = pos;
        }
    }

    return static_cast<Common::UltrasonicPos>(minPos);
}

void UltrasonicTask::executeMessage() {
    switch (msg.getCode()) {

    case Message::CODE::RelEnvEn:
        sendRelEnvEnabled = static_cast<bool>(msg.getDataAsInt());
        communicatorTask.setMessageToSend(Message::ACK, getTaskId());
        break;

    case Message::CODE::ACK_:
    case Message::CODE::Speed:
    case Message::CODE::SteeringAngle:
    case Message::CODE::DriveMode:
    case Message::CODE::RelEnvPoint:
    case Message::CODE::EnvGridEn:
    case Message::CODE::EnvGrid:
    case Message::CODE::NUM_CODES:
        break;
    }
}

/*
Timer2 interrupt calls this function every 24uS where we check the ping status.
*/
void rc_car::ultrasonicEchoCheckIT() {
    ultrasonicTask.echoCheck();
}
