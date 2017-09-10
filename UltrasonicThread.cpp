#include "UltrasonicThread.hpp"

extern UltrasonicThread ultrasonicThread;

UltrasonicThread::UltrasonicThread() : PeriodicThread(PT_PERIOD_TIME_ULTRASONIC, PT_WATCHDOG_TIMEOUT_ULTRASONIC) {
}

void UltrasonicThread::__initialize() {
	ultrasonic.initialize();
}

void UltrasonicThread::__run() {
	// gets next ultrasonic sensor distance (will run in background and call an IT routine)
	if (ultrasonic.isEnabled() && !ultrasonic.isBusy()) {

		// checks if all the ultrasonic sensors have been pinged in this cycle
		// and if yes, stores and validates distances
		if (ultrasonic.cycleFinished())
			ultrasonic.validateAndUpdateSensedPoints();

		ultrasonic.pingNextSensor();
		watchdog->restart();
	}
}

void UltrasonicThread::__onTimedOut() {
	ultrasonic.onWatchdoghasTimedOut();
}

void UltrasonicThread::getSensedPoints(Point<float> dest[ULTRA_NUM_SENSORS]) {
	ultrasonic.copyCurrentValidatedDistances(dest);
}

/*
	Timer2 interrupt calls this function every 24uS where we check the ping status.
*/
void ultrasonicEchoCheckIT() {
	ultrasonicThread.ultrasonic.echoCheck();
}
