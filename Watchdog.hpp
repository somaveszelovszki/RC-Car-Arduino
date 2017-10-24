#ifndef WATCHDOG_HPP
#define WATCHDOG_HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Watchdog implementation.
	*/
	class Watchdog {

	public:

	private: 
		const int TIMEOUT;
		unsigned long startTime;

	public:
		Watchdog(int _timeout) : TIMEOUT(_timeout) {}

		void restart() {
			startTime = millis();
		}

		bool hasTimedOut() const {
			return millis() >= startTime + static_cast<unsigned long>(TIMEOUT);
		}

		unsigned long getStartTime() const {
			return startTime;
		}
	};

}
#endif	// WATCHDOG_HPP

