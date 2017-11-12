#ifndef RC_CAR__WATCHDOG__HPP
#define RC_CAR__WATCHDOG__HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Watchdog implementation.
	*/
	class Watchdog {

	public:

	private: 
		const unsigned long TIMEOUT;
		unsigned long startTime;

	public:
		Watchdog(int _timeout) : TIMEOUT(static_cast<unsigned long>(_timeout)) {}

		void restart() {
			startTime = millis();
		}

		bool hasTimedOut() const {
			return millis() >= startTime + TIMEOUT;
		}

		unsigned long getStartTime() const {
			return startTime;
		}
	};

}
#endif // RC_CAR__WATCHDOG__HPP

