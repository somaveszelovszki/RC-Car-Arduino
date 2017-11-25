#ifndef RC_CAR__WATCHDOG__HPP
#define RC_CAR__WATCHDOG__HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Watchdog implementation.
	*/
	class Watchdog {

	public:
        /** @brief Indicates watchdog state.
        */
		enum State {
			STOPPED = 0, RUNNING
		};

	private:
        /** @brief The watchdog's timeout.
        */
		const unsigned long TIMEOUT;

        /** @brief The current start time.
        */
		unsigned long startTime;

        /** @brief The watchdog's state.
        */
		State state;

	public:
        /** @brief Constructor - sets timeout and state.

        @param _timeout The watchdog's timeout.
        @param _state The watchdog's state.
        */
		Watchdog(int _timeout, State _state = RUNNING) : TIMEOUT(static_cast<unsigned long>(_timeout)), state(_state) {}

        /** @brief Restarts watchdog.
        */
		void restart() {
			startTime = millis();
			state = RUNNING;
		}

        /** @brief Stops watchdog.
        */
		void stop() {
			state = STOPPED;
		}

        /** @brief Checks if watchdg has timed out (if elapsed time since last restart is more than the watchdog's timeout).

        @returns Boolean value indicating if watchdog has timed out.
        */
		bool hasTimedOut() const {
			return state && (millis() >= startTime + TIMEOUT);
		}

        /** @brief Gets watchdog's start time (the time of the last restart).

        @returns The watchdog's start time.
        */
		unsigned long getStartTime() const {
			return startTime;
		}
	};
}
#endif // RC_CAR__WATCHDOG__HPP

