#ifndef RC_CAR__ROTARY_TASK__HPP
#define RC_CAR__ROTARY_TASK__HPP

#include "PeriodicTask.hpp"

namespace rc_car {
    /** @brief Handles rotary encoder - reads counter value periodically.
    */
    class RotaryTask : public PeriodicTask {

    private:
        /** @brief Stores rotary sample's data.
        */
        class __Sample {
        public:
            /** @brief Time (relative to program start) in [ms].
            */
            unsigned long time;

            /** @brief Absolute rotary position.
            */
            int pos;

            /** @brief Constructor - does not initialize data.
            */
            __Sample() {}

            /** @brief Constructor - initializes time and position.

            @param _time The time (relative to program start) in [ms].
            @param _pos The absolute rotary position.
            */
            __Sample(unsigned long _time, int _pos) : time(_time), pos(_pos) {}
        };

        /** @brief Stores previous sample - needed to calculate difference.
        */
        __Sample prev;

        /** @brief Stores the calculated speed - gets updated in every cycle.
        This is the output of the process.
        */
        float speed;

        /** @brief Reads rotary position from the data pins.

        @returns The rotary position.
        */
        int readPosition() const;

        /** @brief Enebles/disables counter.

        @param enabled Indicates if counter should be enabled or disabled.
        */
        void setEnabled(bool enabled) {
            digitalWrite(ROT_EN_PIN, static_cast<uint8_t>(enabled));
        }

        /** @brief Updates new rotary position so that position overflow will not affect difference calculations.

        @param pNewPos Pointer to the new rotary position.
        */
        void updateOverflowPos(int *pNewPos) const;

    public:
        /** @brief Constructor - sets task period time and timeout.
        */
        RotaryTask() : PeriodicTask(TASK_PERIOD_TIME_ROT, TASK_WATCHDOG_TIMEOUT_ROTARY) {}

        /** @brief Sets pin mode for data and enable pins, initializes previous sample object and speed.
        NOTE: Compulsory TASK function - initializes task!
        */
        void initialize();

        /** @brief Reads rotary position and updates speed.
        NOTE: Compulsory TASK function - called in every cycle!
        */
        void run();

        /** @brief Restarts timeout check.
        NOTE: Compulsory TASK function - called when task watchdog timed out!
        */
        void onTimedOut() {
            restartTimeoutCheck();
        }

        /** @brief Gets current speed.

        @returns The current speed.
        */
        float getSpeed() const {
            return speed;
        }
    };

}
#endif // RC_CAR__ROTARY_TASK__HPP
