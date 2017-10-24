/*
	Contains main program.
	Receives messages through a Communicator object, reads ultrasonic sensor distances.
	Handles motors according to drive mode.
*/

#include <pt.h>
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

/*
	Handling communicator and sensors uses Prototasks.
	@see http://dunkels.com/adam/pt/
*/
static struct pt communicator_pt;
static struct pt ultrasonic_pt;
static struct pt drive_pt;
static struct pt rotary_pt;

#define PT_TASK_BODY(task)						\
PT_BEGIN(_pt);									\
PT_WAIT_UNTIL(_pt, task.periodTimeReached());	\
task.checkIfTimedOut();							\
task.run();										\
PT_END(_pt);

/** @brief Sends data to and receives data from phone.
*/
static PT_THREAD(communicator_task(struct pt *_pt)) {
	PT_TASK_BODY(communicatorTask);
}

/** @brief Reads and validates sensor values.
*/
static PT_THREAD(ultrasonic_task(struct pt *_pt)) {
	PT_TASK_BODY(ultrasonicTask);
}

/** @brief Executes drive messages - handles servo and DC motors.
*/
static PT_THREAD(drive_task(struct pt *_pt)) {
	PT_TASK_BODY(driveTask);
}

/** @brief Reads rotary encoder values.
*/
static PT_THREAD(rotary_task(struct pt *_pt)) {
	PT_TASK_BODY(rotaryTask);
}

/** @brief Initializes timers and tasks.
*/
void setup() {
	PeriodicTask::initializeTasks();

	//Common::initializeTimer();

	// TODO maybe use millis() instead - skip custom timer manipulations

	PT_INIT(&communicator_pt);
	PT_INIT(&ultrasonic_pt);
	PT_INIT(&drive_pt);
	PT_INIT(&rotary_pt);
}

void loop() {
	communicator_task(&communicator_pt);
	ultrasonic_task(&ultrasonic_pt);
	rotary_task(&rotary_pt);
	drive_task(&drive_pt);
}

/*
	Timer0 interrupt - called in every 1 ms. Decrements watchdogs.
*/
//ISR(TIMER1_COMPB_vect) {
//	Common::incrementMilliSecs();
//	Watchdog::decrementAll();
//}