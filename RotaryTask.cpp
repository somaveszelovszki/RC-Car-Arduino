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

	diff.time = static_cast<unsigned long>(0);
	diff.pos = 0;
}

void RotaryTask::run() {
	_Value current;

	current.time = millis();

	setEnabled(false);
	current.pos = readPosition();
	setEnabled(true);

	int currPos = current.pos;
	updateOverflowPos(&currPos);

	diff.time = current.time - prev.time;
	diff.pos = currPos - prev.pos;

	prev = current;
}

void RotaryTask::onTimedOut() {
	restartTimeoutCheck();
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

void RotaryTask::updateOverflowPos(int *newPos) const {
	int d = *newPos - prev.pos;

	if (abs(d) > ROT_OVERFLOW_PREV_MAX_D_POS) {
		int dp = d + ROT_COUNTER_RES, dm = d - ROT_COUNTER_RES;
		d = abs(dp) <= abs(dm) ? dp : dm;
		*newPos = prev.pos + d;
	}
}

float RotaryTask::getSpeed() const {
	return diff.time > 0 ?
		(1 / MOT_ROT_TR) * MOT_WHEEL_TR
			* (diff.pos / static_cast<float>(ROT_RES * ROT_EVAL_MUL) * CAR_WHEEL_CIRC) / diff.time * SEC_TO_MSEC
		: 0.0f;
}

Common::AccelerationDir RotaryTask::getDirection() const {
	float speed = getSpeed();
	return speed > 0 ? Common::AccelerationDir::FORWARD
		: speed < 0 ? Common::AccelerationDir::BACKWARD
		: Common::AccelerationDir::RELEASE;
}

