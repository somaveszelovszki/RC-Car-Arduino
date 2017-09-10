/*
	Contains main program.
	Receives commands through a Communicator object, reads ultrasonic sensor distances.
	Handles motors according to drive mode.
*/

#include <pt.h>
#include "CommunicatorThread.hpp"
#include "DriveThread.hpp"
#include "UltrasonicThread.hpp"
#include "RotaryThread.hpp"

CommunicatorThread communicatorThread;
DriveThread driveThread;
UltrasonicThread ultrasonicThread;
RotaryThread rotaryThread;

ISR(TIMER0_COMPA_vect);

/*
	Handling communicator and sensors uses Protothreads.
	@see http://dunkels.com/adam/pt/
*/
static struct pt communicator_pt;
static struct pt ultrasonic_pt;
static struct pt drive_pt;
static struct pt rotary_pt;

#define PT_THREAD_BODY(thread)					\
PT_BEGIN(_pt);									\
PT_WAIT_UNTIL(_pt, thread.periodTimeReached());	\
thread.run();									\
thread.checkhasTimedOut();						\
PT_END(_pt);

/** @brief Sends data to and receives data from phone.
*/
static PT_THREAD(communicator_thread(struct pt *_pt)) {
	PT_THREAD_BODY(communicatorThread);
}

/** @brief Reads and validates sensor values.
*/
static PT_THREAD(ultrasonic_thread(struct pt *_pt)) {
	PT_THREAD_BODY(ultrasonicThread);
}

/** @brief Executes drive commands - handles servo and DC motors.
*/
static PT_THREAD(drive_thread(struct pt *_pt)) {
	PT_THREAD_BODY(driveThread);
}

/** @brief Reads rotary encoder values.
*/
static PT_THREAD(rotary_thread(struct pt *_pt)) {
	PT_THREAD_BODY(rotaryThread);
}

/** @brief Initializes timers and threads.
*/
void setup() {

#if __DEBUG
	Serial.begin(115200);
	Serial.println("setup...");
#endif // __DEBUG

	PeriodicThread::initializeThreads();
	Common::initializeTimer();

	PT_INIT(&communicator_pt);
	PT_INIT(&ultrasonic_pt);
	PT_INIT(&drive_pt);
	PT_INIT(&rotary_pt);
}

void loop() {
	communicator_thread(&communicator_pt);
	ultrasonic_thread(&ultrasonic_pt);
	drive_thread(&drive_pt);
	rotary_thread(&rotary_pt);
}

/*
	Timer0 interrupt - called in every 1 ms. Decrements watchdogs.
*/
ISR(TIMER0_COMPA_vect) {
	Common::incrementMilliSecs();
	Watchdog::decrementAll();
}