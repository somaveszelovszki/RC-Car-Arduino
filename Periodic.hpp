#ifndef RC_CAR__PERIODIC__HPP
#define RC_CAR__PERIODIC__HPP

#include "Watchdog.hpp"

namespace rc_car {
    /** @brief Base class for all objects that need to run periodically.
    */
    class Periodic {

    private:
        /** @brief Watchdog for checking periods.
        */
        Watchdog periodWatchdog;

    public:
        /** @brief Constructor - sets period time.

        @param _periodTime The period time.
        */
        Periodic(int _periodTime) : periodWatchdog(_periodTime) {}

        /** @brief Restarts period check.
        */
        void restartPeriodCheck() {
            periodWatchdog.restart();
        }

        /** @brief Checks if period time has been reached.

        @returns Boolean value indicating if period time has been reached.
        */
        bool periodTimeReached() const {
            return periodWatchdog.hasTimedOut();
        }

        /** @brief Gets start time of the previous period - useful when exact time between two periods is needed.

        @returns The start time of the previous period.
        */
        unsigned long getPrevCalledTime() const {
            return periodWatchdog.getStartTime();
        }
    };
}

#endif // RC_CAR__PERIODIC__HPP

