/** @brief Contains main program.
Receives and sends messages, reads ultrasonic sensor distances and motor speed.
Handles motors according to Drive mode.
*/

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"
#include "DriveTask.hpp"

using namespace rc_car;

/** @brief Handles communications.
*/
CommunicatorTask communicatorTask;

/** @brief Handles ultrasonic sensors.
*/
UltrasonicTask ultrasonicTask;

/** @brief Handles rotary encoder - responsible for speed measuring.
*/
RotaryTask rotaryTask;

/** @brief Responsible for motor handling motors, calculating car trajectory and making driving decisions accroding to the current Drive mode.
*/
DriveTask driveTask;

/** @brief Initializes task.
*/
#define INIT_TASK(task)             \
task.initialize();

/** @brief Checks if task has timed out.
If yes, calls onTimedOut(), if not, calls run() and restarts watchdogs.
*/
#define RUN_TASK(task)              \
if(task.periodTimeReached()) {      \
    if (task.hasTimedOut())         \
        task.onTimedOut();          \
    else {                          \
        wdt_reset();                \
        task.run();                 \
        task.restartPeriodCheck();  \
        task.restartTimeoutCheck(); \
    }                               \
}

/** @brief Initializes tasks.
*/
void setup() {
#if __DEBUG__
    Serial.begin(9600);
    while (!Serial) {}
    DEBUG_println("setup...");
    Serial.flush();
#endif // __DEBUG__

    INIT_TASK(communicatorTask);
    INIT_TASK(ultrasonicTask);
    INIT_TASK(rotaryTask);
    INIT_TASK(driveTask);

    wdt_enable(WDTO_500MS);
}

void loop() {
    RUN_TASK(communicatorTask);
    RUN_TASK(ultrasonicTask);
    RUN_TASK(rotaryTask);
    RUN_TASK(driveTask);
}