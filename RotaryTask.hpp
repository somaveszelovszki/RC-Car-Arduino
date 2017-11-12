#ifndef RC_CAR__ROTARY_TASK__HPP
#define RC_CAR__ROTARY_TASK__HPP

#include "PeriodicTask.hpp"

namespace rc_car {
	/** @brief Handles rotary encoder - reads counter value periodically.
	*/
	class RotaryTask : public PeriodicTask {

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

		int readPosition() const;

		void setEnabled(bool enabled) {
			digitalWrite(ROT_EN_PIN, static_cast<uint8_t>(enabled));
		}

		void updateOverflowPos(int *newPos) const;

	public:
		RotaryTask() : PeriodicTask(PT_PERIOD_TIME_ROT, PT_WATCHDOG_TIMEOUT_ROTARY) {}

		void initialize();
		void run();
		void onTimedOut();

		float getSpeed() const;

		Common::AccelerationDir getDirection() const;
	};

}
#endif // RC_CAR__ROTARY_TASK__HPP
