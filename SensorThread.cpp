#include "SensorThread.hpp"

SensorThread *sensorThread = new SensorThread();

void SensorThread::onTimedOut() {
	ultrasonic->onWatchdogTimedOut();
}

SensorThread::SensorThread() : PeriodicThread(PT_PERIOD_TIME_SENSOR) {
	this->ultrasonic = new Ultrasonic();
}

void SensorThread::initialize() {
	this->ultrasonic->initialize();
}

void SensorThread::run() {
	// gets next ultrasonic sensor distance (will run in background and call an IT routine)
	if (ultrasonic->isEnabled() && !ultrasonic->isBusy()) {

		// checks if all the ultrasonic sensors have been pinged in this cycle
		// and if yes, stores and validates distances
		if (ultrasonic->cycleFinished())
			ultrasonic->updateDistances();

		ultrasonic->pingNextSensor();
		watchdog->restart();
	}
}

void SensorThread::watchdogDecrement() {
	ultrasonic->getWatchdog()->decrement();
}

/*
	Timer2 interrupt calls this function every 24uS where we check the ping status.
*/
void ultrasonicEchoCheckIT() {
	sensorThread->ultrasonic->echoCheck();
}
