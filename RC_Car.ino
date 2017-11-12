/** @brief Contains main program.
Receives and sends messages through the Communicator task, reads ultrasonic sensor distances and motor speed.
Handles motors according to Drive mode.
*/

#include "CommunicatorTask.hpp"
#include "UltrasonicTask.hpp"
#include "RotaryTask.hpp"
#include "DriveTask.hpp"

using namespace rc_car;

CommunicatorTask communicatorTask;
UltrasonicTask ultrasonicTask;
RotaryTask rotaryTask;
DriveTask driveTask;

#define INIT_TASK(task)					\
task.initialize();

#define RUN_TASK(task)					\
if(task.periodTimeReached()) {			\
	if (task.hasTimedOut())				\
		task.onTimedOut();				\
	else {								\
		task.run();						\
		task.restartPeriodCheck();		\
		task.restartTimeoutCheck();		\
	}									\
}

/** @brief Initializes tasks.
*/
void setup() {
	INIT_TASK(communicatorTask);
	INIT_TASK(ultrasonicTask);
	INIT_TASK(rotaryTask);
	INIT_TASK(driveTask);
}

void loop() {
	RUN_TASK(communicatorTask);
	RUN_TASK(ultrasonicTask);
	RUN_TASK(rotaryTask);
	RUN_TASK(driveTask);
}