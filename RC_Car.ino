
/*
	Contains main program.
	Receives commands through a Communicator object, reads ultrasonic sensor distances.
	Handles motors according to drive mode.
*/

#include <pt.h>
#include "Communicator.hpp"
#include "DriveController.hpp"
#include "SensorHandler.hpp"

// counts loop cycles
static unsigned long CYCLE_COUNTER = 0;

extern RotaryEncoder *motorRotaryEncoder;

Communicator *communicator = new Communicator();
DriveController *driveController = new DriveController(motorRotaryEncoder);
extern SensorHandler *sensorHandler;

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

int invalidCntr = 0;

/*
	Sends data to and receives data from phone.
*/
static PT_THREAD(communicatorThread(struct pt *pt)) {
	PT_BEGIN(pt);

	PT_WAIT_UNTIL(pt, communicator->periodCycleThresholdReached(CYCLE_COUNTER));

	// reads available characters - if command end was found, returns true
	
	//motorRotaryEncoder->disableInterrupts();

	if (communicator->receiveChars() ) {

		//motorRotaryEncoder->enableInterrupts();

		Command *receivedCommand = communicator->fetchCommand();

		Serial.println(receivedCommand->toString());

		if (receivedCommand->isValid()) {
				
			switch (receivedCommand->getCode()) {

			case Command::CODE::Speed:
				driveController->executeCommand_Speed(*receivedCommand);
				break;

			case Command::CODE::SteeringAngle:
				driveController->executeCommand_SteeringAngle(*receivedCommand);
				break;

			case Command::CODE::ServoRecalibrate:
				driveController->executeCommand_ServoRecalibrate(*receivedCommand);
				break;

			case Command::CODE::DriveMode:
				driveController->executeCommand_DriveMode(*receivedCommand);
				break;
			}
			invalidCntr = 0;
		} else {
			Serial.println("invalid command received");
			
			if (++invalidCntr >= 3)
				driveController->releaseMotor();
		}

		delete receivedCommand;
	} /*else
		motorRotaryEncoder->enableInterrupts();*/

	// checks if communication timed out
	if (communicator->getWatchdog()->timedOut()) {
		onCommunicationTimedOut();
	}

	// resets watchdog timer used for checking if connection timed out
	communicator->getWatchdog()->restart();

	PT_END(pt);
}

/*
	Reads, valdiates and stores sensors' data.
*/
static PT_THREAD(sensorThread(struct pt *pt)) {
	PT_BEGIN(pt);

	PT_WAIT_UNTIL(pt, sensorHandler->periodCycleThresholdReached(CYCLE_COUNTER));

	driveController->updateValues();

	// gets next ultrasonic sensor distance (will run in background and call an IT routine)
	if (sensorHandler->ultrasonic->isEnabled() && !sensorHandler->ultrasonic->isBusy()) {

		// checks if all the ultrasonic sensors have been pinged in this cycle
		if (sensorHandler->ultrasonic->cycleFinished()) {

			// stores and validates distances
			sensorHandler->ultrasonic->updateDistances();

			unsigned long distances[ULTRASONIC_NUM_SENSORS];

			sensorHandler->ultrasonic->copyCurrentValidatedDistances(distances);

			// sets driving parameters (motors) according to measured distances
			driveController->handleDistanceData(distances);
		}

		sensorHandler->ultrasonic->pingNextSensor();
	}

	// checks if ping timed out - if yes, next sensor needs to be pinged
	if (sensorHandler->ultrasonic->getWatchdog()->timedOut()) {
		sensorHandler->ultrasonic->onWatchdogTimedOut();
	}

	PT_END(pt);
}

/*
	Initializes variables.
*/
void setup() {

	Serial.begin(115200);

	Serial.println("setup...");

	communicator->initialize();
	driveController->initialize();
	sensorHandler->initialize();

	Common::initTimer();

	PT_INIT(&communicator_pt);
	PT_INIT(&sensor_pt);

	driveController->setMode(DriveController::MODE::SAFE_DRIVE);		// TODO set from phone
}

/*
	Runs in every cycle (continuously until program is shut down).
	Calls threads.
*/
void loop() {

	++CYCLE_COUNTER;

	communicatorThread(&communicator_pt);
	sensorThread(&sensor_pt);
}


/*
	Timer0 interrupt - called in every 1 ms. Decrements watchdogs.
*/
ISR(TIMER0_COMPA_vect) {

	++Common::MILLI_SEC_COUNTER;

	// decreases watchdog timers

	communicator->getWatchdog()->decrement();
	sensorHandler->watchdogDecrement();
	driveController->watchdogDecrement();

	driveController->stopTimer = driveController->stopTimer > 0 ? driveController->stopTimer - 1 : 0;
}


void onCommunicationTimedOut() {
	// TODO
	driveController->releaseMotor();

	Serial.println("communicator timed out");

	communicator->getWatchdog()->restart();
}