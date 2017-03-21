#include "SensorHandler.h"

SensorHandler* sensorHandler;

SensorHandler::Ultrasonic::UltrasonicSensor::UltrasonicSensor(uint8_t trigger_pin, uint8_t echo_pin) : NewPing(trigger_pin, echo_pin, ULTRASONIC_MAX_DISTANCE) {
}

SensorHandler::Ultrasonic::Ultrasonic() {
	setEnabled(false);
}

void SensorHandler::Ultrasonic::initialize() {

	// initializes measured distances array
	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		measuredDistances[sensorPos] = ULTRASONIC_MAX_DISTANCE;
		responsive[sensorPos] = true;
	}

	// initializes stored distances
	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		for (unsigned int sampleIndex = 0; sampleIndex < ULTRASONIC_NUM_DISTANCE_SAMPLES; ++sampleIndex) {
			storedDistances[sensorPos][sampleIndex] = ULTRASONIC_MAX_DISTANCE;
		}
	}

	// initializes validated distances
	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		for (unsigned int sampleIndex = 0; sampleIndex < ULTRASONIC_NUM_DISTANCE_SAMPLES; ++sampleIndex) {
			validatedDistances[sensorPos][sampleIndex] = ULTRASONIC_MAX_DISTANCE;
		}
	}

	setEnabled(true);
	busy = false;
	currentSensorPos = (Common::POSITION)(ULTRASONIC_NUM_SENSORS - 1);
	currentSampleIndex = ULTRASONIC_NUM_DISTANCE_SAMPLES - 1;
}

bool SensorHandler::Ultrasonic::isBusy() {
	return busy;
}

bool SensorHandler::Ultrasonic::isEnabled() {
	return enabled;
}

void SensorHandler::Ultrasonic::setEnabled(bool enabled) {
	this->enabled = enabled;
}


void SensorHandler::Ultrasonic::pingNextSensor() {

	// double checks if ultrasonic is enabled
	if (!enabled) return;

	// waits while ultrasonic is busy
	while (busy) {}

	if (cycleFinished()) {
		currentSampleIndex = (currentSampleIndex + 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;

		/*for (unsigned int i = 0; i < ULTRASONIC_NUM_SENSORS; ++i) {

			char s[4];
			Serial.print(itoa(measuredDistances[i], s, 10));
			Serial.print(", ");
		}
		Serial.println("");*/
	}

	// increases sensor position
	currentSensorPos = (Common::POSITION)((int)(currentSensorPos + 1) % ULTRASONIC_NUM_SENSORS);

	// if sensor is not responsive, does not ping it
	if (!responsive[currentSensorPos]) return;

	busy = true;
	sensors[currentSensorPos].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
	measuredDistances[currentSensorPos] = ULTRASONIC_MAX_DISTANCE;                      // in case there's no ping echo for this sensor.
	sensors[currentSensorPos].ping_timer(ultrasonicEchoCheckIT); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
	watchdog->restart();
}

void SensorHandler::Ultrasonic::echoCheck() { // If ping received, set the sensor distance to array.

	if (sensors[currentSensorPos].check_timer()) {

		watchdog->reset();

		measuredDistances[currentSensorPos] = sensors[currentSensorPos].ping_result / US_ROUNDTRIP_CM;

		if (measuredDistances[currentSensorPos] == 0) {
			measuredDistances[currentSensorPos] = ULTRASONIC_MAX_DISTANCE;
		}

		busy = false;
	}
}

void SensorHandler::Ultrasonic::updateDistances() {
	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		updateDistances((Common::POSITION) sensorPos);
	}

	/*Serial.print("stored: ");
	Serial.print(storedDistances[Common::POSITION::FRONT_LEFT][currentSensorPos]);
	Serial.print("\t\t");
	Serial.print("validated: ");
	Serial.println(validatedDistances[Common::POSITION::FRONT_LEFT][currentSensorPos]);*/
}

void SensorHandler::Ultrasonic::updateDistances(Common::POSITION sensorPos) {
	// defines how many stored measured values need to be ULTRASONIC_MAX_DISTANCE, so that we can validate value
	//		->	many times the sensors do not respond, which equals ULTRASONIC_MAX_DISTANCE as a value,
	//			so we must not believe it unless the result is the same for a few times in a row
	static unsigned int maxDistanceValidationSampleNum = 5;		// -> if the result has been ULTRASONIC_MAX_DISTANCE for 2 times in a row, we believe it
	

	// if measured distance is ULTRASONIC_MAX_DISTANCE, validates it
	if (measuredDistances[sensorPos] == ULTRASONIC_MAX_DISTANCE) {
		unsigned int maxDistanceSampleCount = 0;

		// iterates through stored values (from latest to earliest) and counts ULTRASONIC_MAX_DISTANCE values
		// latest (validationSampleNum - 1) results should be ULTRASONIC_MAX_DISTANCE for us to believe that it is valid
		for (unsigned int sampleIndex = (currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;
			sampleIndex != currentSampleIndex; /*no increment*/) {

			if (storedDistances[sensorPos][sampleIndex] == ULTRASONIC_MAX_DISTANCE) {
				++maxDistanceSampleCount;
			} else {
				break;
			}

			sampleIndex = (sampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;
		}

		// validation OK -> we believe that measured distance is really ULTRASONIC_MAX_DISTANCE
		if (maxDistanceSampleCount >= maxDistanceValidationSampleNum - 1) {
			validatedDistances[sensorPos][currentSampleIndex] = ULTRASONIC_MAX_DISTANCE;
		} else {	// validation FAILED -> validated value will be previous not ULTRASONIC_MAX_DISTANCE value

			validatedDistances[sensorPos][currentSampleIndex] = storedDistances[sensorPos][
				(currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1 - maxDistanceSampleCount) % ULTRASONIC_NUM_DISTANCE_SAMPLES
			];
		}

	} else {
		validatedDistances[sensorPos][currentSampleIndex] = measuredDistances[sensorPos];
	}

	storedDistances[sensorPos][currentSampleIndex] = measuredDistances[sensorPos];

	/*if (sensorPos == 0) {
		Serial.print("stored: ");
		Serial.print(storedDistances[sensorPos][currentSampleIndex]);
		Serial.print("\t\t");
		Serial.print("valid: ");
		Serial.println(validatedDistances[sensorPos][currentSampleIndex]);
	}*/

}

void SensorHandler::Ultrasonic::copyCurrentValidatedDistances(unsigned long dest[ULTRASONIC_NUM_SENSORS]) {

	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		dest[sensorPos] = validatedDistances[sensorPos][currentSampleIndex];
	}
}

bool SensorHandler::Ultrasonic::cycleFinished() {
	return currentSensorPos == ULTRASONIC_NUM_SENSORS - 1;
}

Watchdog* SensorHandler::Ultrasonic::getWatchdog() const {
	return watchdog;
}

void SensorHandler::Ultrasonic::onWatchdogTimedOut() {
	//responsive[currentSensorPos] = false;
	sensors[currentSensorPos].timer_stop();

	/*Serial.print("not responsive: ");
	char s[1];
	Serial.println(itoa((int)currentSensorPos, s, 10));*/
	busy = false;
}



SensorHandler::SensorHandler() : PeriodicThread(DEFAULT_CYCLE_PERIOD) {
	this->ultrasonic = new Ultrasonic();
}

void SensorHandler::initialize() {
	this->ultrasonic->initialize();
}

/*
	Timer2 interrupt calls this function every 24uS where we check the ping status.
*/
void ultrasonicEchoCheckIT() {
	sensorHandler->ultrasonic->echoCheck();
}