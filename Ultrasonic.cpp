#include "Ultrasonic.hpp"

Ultrasonic::Ultrasonic() {
	setEnabled(false);
}

const Common::ValidationData Ultrasonic::maxDistanceValidationData = {
	VALIDATION_MAX_DIST_SAMPLE_NUM, VALIDATION_MAX_DIST_RELATIVE_ERROR
};

const Common::ValidationData Ultrasonic::defaultValidationData = {
	VALIDATION_DEFAULT_SAMPLE_NUM, VALIDATION_DEFAULT_RELATIVE_ERROR
};

void Ultrasonic::initialize() {

	pinMode(ULTRASONIC_SEL_0_PIN, OUTPUT);
	pinMode(ULTRASONIC_SEL_1_PIN, OUTPUT);
	pinMode(ULTRASONIC_SEL_2_PIN, OUTPUT);
	pinMode(ULTRASONIC_SEL_3_PIN, OUTPUT);

	// initializes measured distances array
	for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		measuredDistances[sensorPos] = ULTRASONIC_MAX_DISTANCE;
		responsive[sensorPos] = true;
	}

	// initializes stored distances
	for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		for (int sampleIndex = 0; sampleIndex < ULTRASONIC_NUM_DISTANCE_SAMPLES; ++sampleIndex) {
			storedDistances[sensorPos][sampleIndex] = ULTRASONIC_MAX_DISTANCE;
		}
	}

	// initializes validated distances
	for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		for (int sampleIndex = 0; sampleIndex < ULTRASONIC_NUM_DISTANCE_SAMPLES; ++sampleIndex) {
			validatedDistances[sensorPos][sampleIndex] = ULTRASONIC_MAX_DISTANCE;
		}
	}

	setEnabled(true);
	busy = false;
	currentSensorPos = (Ultrasonic::POSITION)(ULTRASONIC_NUM_SENSORS - 1);
	currentSampleIndex = ULTRASONIC_NUM_DISTANCE_SAMPLES - 1;
}

bool Ultrasonic::isBusy() {
	return busy;
}

bool Ultrasonic::isEnabled() {
	return enabled;
}

void Ultrasonic::setEnabled(bool enabled) {
	this->enabled = enabled;
}


void Ultrasonic::pingNextSensor() {

	// double checks if ultrasonic is enabled
	if (!enabled) return;

	// waits while ultrasonic is busy
	while (busy) {}

	if (cycleFinished()) {
		currentSampleIndex = (currentSampleIndex + 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;

		/*for (int i = 0; i < ULTRASONIC_NUM_SENSORS; ++i) {

		char s[4];
		Serial.print(itoa(measuredDistances[i], s, 10));
		Serial.print(", ");
		}
		Serial.println("");*/
	}

	// increases sensor position
	currentSensorPos = (Ultrasonic::POSITION)((int)(currentSensorPos + 1) % ULTRASONIC_NUM_SENSORS);

	// if sensor is not responsive, does not ping it
	//if (!responsive[currentSensorPos]) return;

	busy = true;
	sensorConnection->timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
	measuredDistances[currentSensorPos] = ULTRASONIC_MAX_DISTANCE;                      // in case there's no ping echo for this sensor.
	updateSensorSelection();
	sensorConnection->ping_timer(ultrasonicEchoCheckIT); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
}

void Ultrasonic::echoCheck() { // If ping received, set the sensor distance to array.

	if (sensorConnection->check_timer()) {
		measuredDistances[currentSensorPos] = sensorConnection->ping_result / US_ROUNDTRIP_CM;

		if (measuredDistances[currentSensorPos] == 0)
			measuredDistances[currentSensorPos] = ULTRASONIC_MAX_DISTANCE;

		busy = false;
	}
}

void Ultrasonic::updateDistances() {
	for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos) {
		updateDistances((Ultrasonic::POSITION) sensorPos);
		//Serial.print(validatedDistances[sensorPos][currentSampleIndex]);
		//if (sensorPos != ULTRASONIC_NUM_SENSORS - 1) Serial.print(", ");
	}

	//Serial.println();

	//Serial.print("stored: ");
	//Serial.print(storedDistances[Ultrasonic::POSITION::FRONT_LEFT][currentSampleIndex]);
	//Serial.print("\t\t");
	//Serial.print("validated: ");
	//Serial.println(validatedDistances[Ultrasonic::POSITION::FRONT_LEFT][currentSampleIndex]);
}

void Ultrasonic::updateDistances(Ultrasonic::POSITION sensorPos) {

	storedDistances[sensorPos][currentSampleIndex] = measuredDistances[sensorPos];

	// if measured distance is ULTRASONIC_MAX_DISTANCE, validates it
	if (measuredDistances[sensorPos] == ULTRASONIC_MAX_DISTANCE) {
		validate(sensorPos, maxDistanceValidationData);

	} else {
		validate(sensorPos, defaultValidationData);
	}
}

void Ultrasonic::validate(Ultrasonic::POSITION sensorPos, Common::ValidationData validationData) {

	int prevValidatedValue = validatedDistances[sensorPos][(currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES];
	int currentStoredValue = storedDistances[sensorPos][currentSampleIndex];

	/*
	Checks if current stored value is in the given range of the previous validated value for the sensor.
	e.g. if the previous validated value was 150, we will believe 145 without hesitation, no need for further validation
	*/
	if (Common::isInRange(prevValidatedValue, currentStoredValue, validationData.relativeError, Common::ERROR_SIGN::BOTH)) {
		validatedDistances[sensorPos][currentSampleIndex] = currentStoredValue;
		return;
	}

	// if we got here, current stored value is out of range of the previous validated value so we need to validate

	int sampleCount = 0;

	// iterates through stored values (from latest to earliest) and counts how many values are also out of range of previous validated value (sign of error is also important to match)
	// latest (validationSampleNum - 1) results should be out of range for us to believe that current value is valid
	for (int sampleIndex = (currentSampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES;
		sampleIndex != currentSampleIndex;
		sampleIndex = (sampleIndex + ULTRASONIC_NUM_DISTANCE_SAMPLES - 1) % ULTRASONIC_NUM_DISTANCE_SAMPLES) {

		if (!Common::isInRange(prevValidatedValue, storedDistances[sensorPos][sampleIndex], validationData.relativeError,
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

void Ultrasonic::updateSensorSelection() {

	uint8_t pos = (uint8_t)currentSensorPos;

	// writes position to selection wires (as binary value)
	digitalWrite(ULTRASONIC_SEL_0_PIN, (pos & 0x01) != 0 ? HIGH : LOW);
	digitalWrite(ULTRASONIC_SEL_1_PIN, (pos & 0x02) != 0 ? HIGH : LOW);
	digitalWrite(ULTRASONIC_SEL_2_PIN, (pos & 0x04) != 0 ? HIGH : LOW);
	digitalWrite(ULTRASONIC_SEL_3_PIN, (pos & 0x08) != 0 ? HIGH : LOW);
}

void Ultrasonic::copyCurrentValidatedDistances(int dest[ULTRASONIC_NUM_SENSORS]) {
	noInterrupts();
	for (int sensorPos = 0; sensorPos < ULTRASONIC_NUM_SENSORS; ++sensorPos)
		dest[sensorPos] = validatedDistances[sensorPos][currentSampleIndex];
	interrupts();
}

bool Ultrasonic::cycleFinished() {
	return currentSensorPos == ULTRASONIC_NUM_SENSORS - 1;
}

void Ultrasonic::onWatchdogTimedOut() {
	//responsive[currentSensorPos] = false;
	sensorConnection->timer_stop();

	/*Serial.print("not responsive: ");
	char s[1];
	Serial.println(itoa((int)currentSensorPos, s, 10));*/
	busy = false;
}
