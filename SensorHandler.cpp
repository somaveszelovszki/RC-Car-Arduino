#include "SensorHandler.h"

SensorHandler *sensorHandler;

SensorHandler::Ultrasonic::UltrasonicSensor::UltrasonicSensor(uint8_t trigger_pin, uint8_t echo_pin) : NewPing(trigger_pin, echo_pin, ULTRASONIC_MAX_DISTANCE) {
}

SensorHandler::Ultrasonic::Ultrasonic() {
	setEnabled(false);
}

const SensorHandler::ValidationData SensorHandler::Ultrasonic::maxDistanceValidationData = {
	maxDistanceValidationSampleNum, maxDistanceValidationErrorPercent
};

const SensorHandler::ValidationData SensorHandler::Ultrasonic::defaultValidationData = {
	defaultValidationSampleNum, defaultValidationErrorPercent
};

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
	//if (!responsive[currentSensorPos]) return;

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
		//Serial.print(validatedDistances[sensorPos][currentSampleIndex]);
		//if (sensorPos != ULTRASONIC_NUM_SENSORS - 1) Serial.print(", ");
	}

	//Serial.println();

	//Serial.print("stored: ");
	//Serial.print(storedDistances[Common::POSITION::FRONT_LEFT][currentSampleIndex]);
	//Serial.print("\t\t");
	//Serial.print("validated: ");
	//Serial.println(validatedDistances[Common::POSITION::FRONT_LEFT][currentSampleIndex]);
}

void SensorHandler::Ultrasonic::updateDistances(Common::POSITION sensorPos) {

	storedDistances[sensorPos][currentSampleIndex] = measuredDistances[sensorPos];

	// if measured distance is ULTRASONIC_MAX_DISTANCE, validates it
	if (measuredDistances[sensorPos] == ULTRASONIC_MAX_DISTANCE) {
		validate(sensorPos, maxDistanceValidationData);

	} else {
		validate(sensorPos, defaultValidationData);
	}
}

void SensorHandler::Ultrasonic::validate(Common::POSITION sensorPos, ValidationData validationData) {

	unsigned long prevValidatedValue = validatedDistances[sensorPos][(currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES];
	unsigned long currentStoredValue = storedDistances[sensorPos][currentSampleIndex];

	/*
		Checks if current stored value is in the given range of the previous validated value for the sensor.
		e.g. if the previous validated value was 150, we will believe 145 without hesitation, no need for further validation
	*/
	if (isInRange(prevValidatedValue, currentStoredValue, validationData.errorPercent, Common::ERROR_SIGN::BOTH)) {
		validatedDistances[sensorPos][currentSampleIndex] = currentStoredValue;
		return;
	}

	// if we got here, current stored value is out of range of the previous validated value so we need to validate

	unsigned int sampleCount = 0;

	// iterates through stored values (from latest to earliest) and counts how many values are also out of range of previous validated value (sign of error is also important to match)
	// latest (validationSampleNum - 1) results should be out of range for us to believe that current value is valid
	for (unsigned int sampleIndex = (currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;
		sampleIndex != currentSampleIndex;
		sampleIndex = (sampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES) {

		if (!isInRange(prevValidatedValue, storedDistances[sensorPos][sampleIndex], validationData.errorPercent,
			currentStoredValue < prevValidatedValue ? Common::ERROR_SIGN::NEGATIVE : Common::ERROR_SIGN::POSITIVE)) {
			
			++sampleCount;
		} else {
			break;
		}
	}

	// validation OK -> we believe that distance is really what we measured
	if (sampleCount >= validationData.validationSampleNum - 1) {
		validatedDistances[sensorPos][currentSampleIndex] = currentStoredValue;
	} else {	// validation FAILED -> validated value will be previous validated value

		validatedDistances[sensorPos][currentSampleIndex] = prevValidatedValue;
	}
}

/*
	Checks if value is in a given range of the reference value.
*/
bool SensorHandler::Ultrasonic::isInRange(unsigned long ref, unsigned long value, unsigned int errorPercent, Common::ERROR_SIGN errorDir) {

	unsigned long max = errorDir == Common::ERROR_SIGN::NEGATIVE ?
		ref : ref * (100 + errorPercent) / 100;

	unsigned long min = errorDir == Common::ERROR_SIGN::POSITIVE ?
		ref : ref * (100 - errorPercent) / 100;

	return value <= max && value >= min;
}

void SensorHandler::Ultrasonic::copyCurrentValidatedDistances(unsigned long dest[ULTRASONIC_NUM_SENSORS]) {

	for (unsigned int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		dest[sensorPos] = validatedDistances[sensorPos][currentSampleIndex];
	}
}

bool SensorHandler::Ultrasonic::cycleFinished() {
	return currentSensorPos == ULTRASONIC_NUM_SENSORS - 1;
}

Watchdog *SensorHandler::Ultrasonic::getWatchdog() const {
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

void SensorHandler::watchdogDecrement() {
	ultrasonic->getWatchdog()->decrement();
}

/*
	Timer2 interrupt calls this function every 24uS where we check the ping status.
*/
void ultrasonicEchoCheckIT() {
	sensorHandler->ultrasonic->echoCheck();
}
