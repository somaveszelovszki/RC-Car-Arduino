#ifndef RC_CAR__PERIODIC_TASK__HPP
#define RC_CAR__PERIODIC_TASK__HPP

#include "Watchdog.hpp"
#include "Periodic.hpp"

namespace rc_car {
/** @brief Base class for tasks that run periodically.
*/
class PeriodicTask : public Periodic {

private:
    /** @brief The task id - used as 0-based array index.
    */
    int id;

    /** @brief Number of tasks in the program.
    */
    static int tasksCount;

    /** @brief Watchdog for checking if task timed out (has not run in a given time).
    */
    Watchdog timeoutCheckWatchdog;

public:
    /** @brief Constructor - sets perios time and timeout.

    @param _periodTime The period time.
    @param _watchdogTimeout The maximum task timeout (if task does not run for this long, onTimedOut() will be called).
    */
    PeriodicTask(int _periodTime, int _watchdogTimeout) : Periodic(_periodTime), timeoutCheckWatchdog(_watchdogTimeout) {
        this->id = tasksCount++;
    }

    /** brief Gets the task's id.

    @returns The task id.
    */
    int getTaskId() const {
        return this->id;
    }

    /** @brief Checks if task has timed out.

    @returns Boolean value indicating if task has timed out.
    */
    bool hasTimedOut() {
        return timeoutCheckWatchdog.hasTimedOut();
    }

    /** @brief Restarts timeout check.
    */
    void restartTimeoutCheck() {
        timeoutCheckWatchdog.restart();
    }
};
}

#endif // RC_CAR__PERIODIC_TASK__HPP