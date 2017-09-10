# include "RotaryThread.hpp"

void RotaryThread::__initialize() {
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

	prev.time = Common::milliSecs();
	prev.pos = 0;

	diff.time = static_cast<unsigned long>(0);
	diff.pos = 0;
}

void RotaryThread::__run() {
	_Value current;

	current.time = Common::milliSecs();

	setEnabled(false);
	current.pos = readPosition();
	setEnabled(true);

	updateOverflowPos(&current.pos);

	diff.time = current.time - prev.time;
	diff.pos = current.pos - prev.pos;

	prev = current;

	/*Serial.print("time: ");
	Serial.print((int)storedValue.d_time);
	Serial.print("\tpos: ");
	Serial.println((int)storedValue.d_pos);*/
}

void RotaryThread::__onTimedOut() {
	// TODO
}

int RotaryThread::readPosition() const {
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

void RotaryThread::updateOverflowPos(int *newPos) const {
	int d = *newPos - prev.pos;

	if (abs(d) > ROTARY_OVERFLOW_PREV_MAX_D_POS) {
		int dp = d + 256, dm = d - 256;
		d = abs(dp) <= abs(dm) ? dp : dm;
		*newPos = prev.pos + d;
	}
}

float RotaryThread::getSpeed() const {
	return (1 / MOTOR_ROTARY_TRANSFER_RATE) * MOTOR_WHEEL_TRANSFER_RATE
		* ((diff.pos / static_cast<float>(ROTARY_RESOLUTION)) * WHEEL_CIRCUMFERENCE) / diff.time * SEC_TO_MSEC;
}

Common::AccelerationDir RotaryThread::getDirection() const {
	float speed = getSpeed();
	return speed > 0 ? Common::AccelerationDir::FORWARD
		: speed < 0 ? Common::AccelerationDir::BACKWARD
		: Common::AccelerationDir::RELEASE;
}

