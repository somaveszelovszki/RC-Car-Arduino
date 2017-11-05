/*
	Contains main program.
	Receives messages through a Communicator object, reads ultrasonic sensor distances.
	Handles motors according to drive mode.
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

//ISR(TIMER1_COMPB_vect);

#define INIT_TASK(task)			\
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

	//Common::initializeTimer();

	// TODO maybe use millis() instead - skip custom timer manipulations

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

/*
	Timer0 interrupt - called in every 1 ms. Decrements watchdogs.
*/
//ISR(TIMER1_COMPB_vect) {
//	Common::incrementMilliSecs();
//	Watchdog::decrementAll();
//}