#ifndef RC_CAR__WATCHDOG__HPP
#define RC_CAR__WATCHDOG__HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Watchdog implementation.
	*/
	class Watchdog {

	public:
		enum State {
			STOPPED = 0, RUNNING
		};

	private:
		const unsigned long TIMEOUT;
		unsigned long startTime;
		State state;

	public:
		Watchdog(int _timeout, State _state = RUNNING) : TIMEOUT(static_cast<unsigned long>(_timeout)), state(_state) {}

		void restart() {
			startTime = millis();
			state = RUNNING;
		}

		void stop() {
			state = STOPPED;
		}

		bool hasTimedOut() const {
			return state && (millis() >= startTime + TIMEOUT);
		}

		unsigned long getStartTime() const {
			return startTime;
		}
	};

}
#endif // RC_CAR__WATCHDOG__HPP

