#ifndef ROTARY_THREAD_HPP
#define ROTARY_THREAD_HPP

class RotaryThread : public PeriodicThread {

private:

	class _Value {
	public:
		unsigned long time;	// [ms]
		int pos;

		_Value() {}
		_Value(unsigned long _time, int _pos) : time(_time), pos(_pos) {}
	};

	_Value prev;
	_Value diff;

	void __initialize() override;
	void __run() override;
	void __onTimedOut() override;

	int readPosition() const;

	void setEnabled(bool enabled) {
		digitalWrite(ROTARY_EN_PIN, enabled);
	}

	void updateOverflowPos(int *newPos) const;

public:

	RotaryThread() : PeriodicThread(PT_PERIOD_TIME_ROTARY, PT_WATCHDOG_TIMEOUT_ROTARY) {}

	float getSpeed() const;

	Common::AccelerationDir getDirection() const;
};

#endif // ROTARY_THREAD_HPP
