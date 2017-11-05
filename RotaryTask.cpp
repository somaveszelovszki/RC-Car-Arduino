# include "RotaryTask.hpp"

using namespace rc_car;

void RotaryTask::initialize() {
	// TODO initialize pins
	pinMode(ROTARY_D0_PIN, INPUT);
	pinMode(ROTARY_D1_PIN, INPUT);
	pinMode(ROTARY_D2_PIN, INPUT);
	pinMode(ROTARY_D3_PIN, INPUT);
	pinMode(ROTARY_D4_PIN, INPUT);
	pinMode(ROTARY_D5_PIN, INPUT);
	pinMode(ROTARY_D6_PIN, INPUT);
	pinMode(ROTARY_D7_PIN, INPUT);

	pinMode(ROTARY_EN_PIN, OUTPUT);

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
	// TODO
}

int RotaryTask::readPosition() const {
	return static_cast<int>(static_cast<int8_t>(
		(digitalRead(ROTARY_D7_PIN) << 7)
		+ (digitalRead(ROTARY_D6_PIN) << 6)
		+ (digitalRead(ROTARY_D5_PIN) << 5)
		+ (digitalRead(ROTARY_D4_PIN) << 4)
		+ (digitalRead(ROTARY_D3_PIN) << 3)
		+ (digitalRead(ROTARY_D2_PIN) << 2)
		+ (digitalRead(ROTARY_D1_PIN) << 1)
		+ digitalRead(ROTARY_D0_PIN)));
}

void RotaryTask::updateOverflowPos(int *newPos) const {
	int d = *newPos - prev.pos;

	if (abs(d) > ROTARY_OVERFLOW_PREV_MAX_D_POS) {
		int dp = d + 256, dm = d - 256;
		d = abs(dp) <= abs(dm) ? dp : dm;
		*newPos = prev.pos + d;
	}
}

float RotaryTask::getSpeed() const {
	return diff.time > 0 ?
		(1 / MOTOR_ROTARY_TRANSFER_RATE) * MOTOR_WHEEL_TRANSFER_RATE
			* (diff.pos / static_cast<float>(ROTARY_RESOLUTION * ROTARY_EVAL_MULTIPLIER) * CAR_WHEEL_CIRCUMFERENCE) / diff.time * SEC_TO_MSEC
		: 0.0f;
}

Common::AccelerationDir RotaryTask::getDirection() const {
	float speed = getSpeed();
	return speed > 0 ? Common::AccelerationDir::FORWARD
		: speed < 0 ? Common::AccelerationDir::BACKWARD
		: Common::AccelerationDir::RELEASE;
}

