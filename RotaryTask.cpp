# include "RotaryTask.hpp"

using namespace rc_car;

void RotaryTask::initialize() {
    pinMode(ROT_D0_PIN, INPUT);
    pinMode(ROT_D1_PIN, INPUT);
    pinMode(ROT_D2_PIN, INPUT);
    pinMode(ROT_D3_PIN, INPUT);
    pinMode(ROT_D4_PIN, INPUT);
    pinMode(ROT_D5_PIN, INPUT);
    pinMode(ROT_D6_PIN, INPUT);
    pinMode(ROT_D7_PIN, INPUT);

    pinMode(ROT_EN_PIN, OUTPUT);

    prev.time = millis();
    prev.pos = 0;

    speed = 0.0f;
}

void RotaryTask::run() {
    __Sample current, diff;

    current.time = millis();

    // counter needs to be disabled while reading position
    setEnabled(false);
    current.pos = readPosition();
    setEnabled(true);

    int currPos = current.pos;
    updateOverflowPos(&currPos);

    diff.time = current.time - prev.time;
    diff.pos = currPos - prev.pos;

    prev = current;

    // updates speed
    speed = diff.time > 0 ?
        (1 / MOT_ROT_TR) * MOT_WHEEL_TR * (diff.pos / static_cast<float>(ROT_RES * ROT_EVAL_MUL) * CAR_WHEEL_CIRC) / diff.time * SEC_TO_MSEC
        : 0.0f;
}

int RotaryTask::readPosition() const {
    return static_cast<int>(static_cast<int8_t>(
        (digitalRead(ROT_D7_PIN) << 7)
        + (digitalRead(ROT_D6_PIN) << 6)
        + (digitalRead(ROT_D5_PIN) << 5)
        + (digitalRead(ROT_D4_PIN) << 4)
        + (digitalRead(ROT_D3_PIN) << 3)
        + (digitalRead(ROT_D2_PIN) << 2)
        + (digitalRead(ROT_D1_PIN) << 1)
        + digitalRead(ROT_D0_PIN)));
}

void RotaryTask::updateOverflowPos(int *pNewPos) const {
    int d = *pNewPos - prev.pos;

    if (abs(d) > ROT_OVERFLOW_PREV_MAX_D_POS) {
        int dp = d + ROT_COUNTER_RES, dm = d - ROT_COUNTER_RES;
        d = abs(dp) <= abs(dm) ? dp : dm;
        *pNewPos = prev.pos + d;
    }
}

