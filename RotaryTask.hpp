#ifndef ROTARY_TASK_HPP
#define ROTARY_TASK_HPP

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

		void __initialize() override;
		void __run(void *unused) override;
		void __onTimedOut() override;

		int readPosition() const;

		void setEnabled(bool enabled) {
			digitalWrite(ROTARY_EN_PIN, enabled);
		}

		void updateOverflowPos(int *newPos) const;

	public:

		RotaryTask() : PeriodicTask(PT_PERIOD_TIME_ROTARY, PT_WATCHDOG_TIMEOUT_ROTARY) {}

		float getSpeed() const;

		Common::AccelerationDir getDirection() const;
	};

}
#endif // ROTARY_TASK_HPP
