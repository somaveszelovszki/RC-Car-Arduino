/*
	Contains main program.
	Receives commands through a Communicator object, reads ultrasonic sensor distances.
	Handles motors according to drive mode.
*/

#include <pt.h>
#include "CommunicatorThread.hpp"
#include "DriveThread.hpp"
#include "SensorThread.hpp"

extern RotaryEncoder *motorRotaryEncoder;

CommunicatorThread *communicatorThread = new CommunicatorThread();
DriveThread *driveThread = new DriveThread(motorRotaryEncoder);
extern SensorThread *sensorThread;

ISR(TIMER0_COMPA_vect);

/*
	Called when communication timed out - meaning that communication with phone has stopped.
	Stops car.
*/
void onCommunicationTimedOut();

/*
	Handling communicator and sensors uses Protothreads.
	@see http://dunkels.com/adam/pt/
*/
static struct pt communicator_pt;
static struct pt sensor_pt;
static struct pt drive_pt;

/*
	Sends data to and receives data from phone.
*/
static PT_THREAD(communicator_thread(struct pt *pt)) {
	PT_BEGIN(pt);

	PT_WAIT_UNTIL(pt, communicatorThread->periodTimeReached());

	communicatorThread->start();

	// checks if communication timed out
	communicatorThread->checkTimedOut();

	PT_END(pt);
}

/*
	Reads and validates sensor values.
*/
static PT_THREAD(sensor_thread(struct pt *pt)) {
	PT_BEGIN(pt);

	PT_WAIT_UNTIL(pt, sensorThread->periodTimeReached());

	sensorThread->start();

	// if ping timed out, next sensor needs to be pinged
	sensorThread->checkTimedOut();

	PT_END(pt);
}

/*
	Executes drive commands - handles servo and DC motors.
*/
static PT_THREAD(drive_thread(struct pt *pt)) {
	PT_BEGIN(pt);

	PT_WAIT_UNTIL(pt, driveThread->periodTimeReached());

	sensorThread->ultrasonic->copyCurrentValidatedDistances(driveThread->distances);

	// TODO take into considerations the measured distances
	// TODO drive according to DRIVE MODE
	if (communicatorThread->available())
		driveThread->start(communicatorThread->getCommand());

	PT_END(pt);
}

/*
	Initializes variables.
*/
void setup() {

	Serial.begin(115200);

	Serial.println("setup...");

	PeriodicThread::initializeThreads();

	Common::initTimer();

	PT_INIT(&communicator_pt);
	PT_INIT(&sensor_pt);
	PT_INIT(&drive_pt);
}

/*
	Runs in every cycle (continuously until program is shut down).
	Calls threads.
*/
void loop() {
	communicator_thread(&communicator_pt);
	sensor_thread(&sensor_pt);
	drive_thread(&drive_pt);
}

/*
	Timer0 interrupt - called in every 1 ms. Decrements watchdogs.
*/
ISR(TIMER0_COMPA_vect) {

	Common::incrementMilliSecs();

	PeriodicThread::decrementWatchdogs();

	// TODO This is only a temporary solution!
	driveThread->stopTimer = driveThread->stopTimer > 0 ? driveThread->stopTimer - 1 : 0;
}