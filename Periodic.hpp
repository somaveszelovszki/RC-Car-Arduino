#ifndef PERIODIC_HPP
#define PERIODIC_HPP

#include "Common.hpp"

namespace rc_car {
	/** @brief Base class for all objects that need to run periodically.
	*/
	class Periodic {

	protected:
		int periodTime;		// period time [ms]
		unsigned long prevTime;		// previous time of execution [ms]

	public:
		Periodic(int _periodTime) : periodTime(_periodTime) {
			prevTime = Common::milliSecs();
		}

		bool periodTimeReached() const {
			return Common::milliSecs() - prevTime >= static_cast<unsigned long>(periodTime);
		}
	};
}

#endif // PERIODIC_HPP

