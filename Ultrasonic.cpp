#include "Ultrasonic.hpp"

Ultrasonic::Ultrasonic() {
	sensorConnection = new NewPing(ULTRA_TRIGGER_PIN, ULTRA_ECHO_PIN, ULTRA_MAX_DISTANCE);
	setEnabled(false);

	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT_CORNER)].pos.X = ULTRA_POS_X_FLC;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT_CORNER)].pos.Y = ULTRA_POS_Y_FLC;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT_CORNER)].viewAngle = ULTRA_VIEW_ANGLE_FLC;

	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT)].pos.X = ULTRA_POS_X_FL;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT)].pos.Y = ULTRA_POS_Y_FL;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_LEFT)].viewAngle = ULTRA_VIEW_ANGLE_FL;

	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT)].pos.X = ULTRA_POS_X_FR;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT)].pos.Y = ULTRA_POS_Y_FR;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT)].viewAngle = ULTRA_VIEW_ANGLE_FR;

	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT_CORNER)].pos.X = ULTRA_POS_X_FRC;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT_CORNER)].pos.Y = ULTRA_POS_Y_FRC;
	sensors[static_cast<int>(Common::UltrasonicPos::FRONT_RIGHT_CORNER)].viewAngle = ULTRA_VIEW_ANGLE_FRC;



	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT_CORNER)].pos.X = ULTRA_POS_X_RLC;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT_CORNER)].pos.Y = ULTRA_POS_Y_RLC;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT_CORNER)].viewAngle = ULTRA_VIEW_ANGLE_RLC;

	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT)].pos.X = ULTRA_POS_X_RL;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT)].pos.Y = ULTRA_POS_Y_RL;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_LEFT)].viewAngle = ULTRA_VIEW_ANGLE_RL;

	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT)].pos.X = ULTRA_POS_X_RR;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT)].pos.Y = ULTRA_POS_Y_RR;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT)].viewAngle = ULTRA_VIEW_ANGLE_RR;

	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT_CORNER)].pos.X = ULTRA_POS_X_RRC;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT_CORNER)].pos.Y = ULTRA_POS_Y_RRC;
	sensors[static_cast<int>(Common::UltrasonicPos::REAR_RIGHT_CORNER)].viewAngle = ULTRA_VIEW_ANGLE_RRC;



	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_FRONT)].pos.X = ULTRA_POS_X_LF;
	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_FRONT)].pos.Y = ULTRA_POS_Y_LF;
	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_FRONT)].viewAngle = ULTRA_VIEW_ANGLE_LF;

	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_REAR)].pos.X = ULTRA_POS_X_LR;
	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_REAR)].pos.Y = ULTRA_POS_Y_LR;
	sensors[static_cast<int>(Common::UltrasonicPos::LEFT_REAR)].viewAngle = ULTRA_VIEW_ANGLE_LR;

	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_FRONT)].pos.X = ULTRA_POS_X_RF;
	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_FRONT)].pos.Y = ULTRA_POS_Y_RF;
	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_FRONT)].viewAngle = ULTRA_VIEW_ANGLE_RF;

	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_REAR)].pos.X = ULTRA_POS_X_RiR;
	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_REAR)].pos.Y = ULTRA_POS_Y_RiR;
	sensors[static_cast<int>(Common::UltrasonicPos::RIGHT_REAR)].viewAngle = ULTRA_VIEW_ANGLE_RiR;
}

const Common::ValidationData Ultrasonic::maxDistanceValidationData = {
	ULTRA_VALID_MAX_DIST_SAMPLE_NUM, ULTRA_VALID_MAX_DIST_RELATIVE_ERROR
};

const Common::ValidationData Ultrasonic::defaultValidationData = {
	ULTRA_VALID_DEFAULT_SAMPLE_NUM, ULTRA_VALID_DEFAULT_RELATIVE_ERROR
};

void Ultrasonic::initialize() {

	pinMode(ULTRA_SEL_0_PIN, OUTPUT);
	pinMode(ULTRA_SEL_1_PIN, OUTPUT);
	pinMode(ULTRA_SEL_2_PIN, OUTPUT);
	pinMode(ULTRA_SEL_3_PIN, OUTPUT);

	// initializes sensor data - measured, stored and validatied distances, nonResponsiveCounter
	for (int pos = 0; pos < ULTRA_NUM_SENSORS; ++pos) {
		sensors[pos].dist_measured = ULTRA_MAX_DISTANCE;

		for (int sampleIndex = 0; sampleIndex < ULTRA_NUM_DIST_SAMPLES; ++sampleIndex) {
			sensors[pos].dist_stored[sampleIndex] = ULTRA_MAX_DISTANCE;
			sensors[pos].dist_validated[sampleIndex] = ULTRA_MAX_DISTANCE;
		}

		sensors[pos].nonResponsiveCounter = 0;
	}

	setEnabled(true);
	busy = false;
	currentSensorPos = static_cast<Common::UltrasonicPos>(ULTRA_NUM_SENSORS - 1);
	currentSampleIndex = ULTRA_NUM_DIST_SAMPLES - 1;
}

bool Ultrasonic::isBusy() const {
	return busy;
}

bool Ultrasonic::isEnabled() const {
	return enabled;
}

void Ultrasonic::setEnabled(bool enabled) {
	this->enabled = enabled;
}


void Ultrasonic::pingNextSensor() {
	if (cycleFinished()) {
		currentSampleIndex = (currentSampleIndex + 1) % ULTRA_NUM_DIST_SAMPLES;

		/*for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {

			char s[4];
			Serial.print(itoa(measuredDistances[i], s, 10));
			Serial.print(", ");
		}
		Serial.println("");*/
	}

	// increases sensor position
	currentSensorPos = static_cast<Common::UltrasonicPos>((static_cast<int>(currentSensorPos) + 1) % ULTRA_NUM_SENSORS);

	// if sensor is not responsive, does not ping it
	if (sensors[static_cast<int>(currentSensorPos)].nonResponsiveCounter <= ULTRA_VALID_MAX_NON_RESPONSIVE_COUNT) {
		busy = true;
		sensorConnection->timer_stop();
		sensors[static_cast<int>(currentSensorPos)].dist_measured = ULTRA_MAX_DISTANCE;	// in case there is no echo
		updateSensorSelection();
		// Pings sensor (processing continues, interrupt will call echoCheck to look for echo).
		sensorConnection->ping_timer(ultrasonicEchoCheckIT);
	}

	}

void Ultrasonic::echoCheck() { // If ping received, set the sensor distance to array.

	if (sensorConnection->check_timer()) {
		sensors[static_cast<int>(currentSensorPos)].dist_measured = sensorConnection->ping_result / US_ROUNDTRIP_CM;

		// sensor response is 0 if (measured distance > ULTRA_MAX_DISTANCE)
		if (sensors[static_cast<int>(currentSensorPos)].dist_measured == 0)
			sensors[static_cast<int>(currentSensorPos)].dist_measured = ULTRA_MAX_DISTANCE;

		busy = false;
	}
}

void Ultrasonic::validateAndUpdateSensedPoints() {
	for (int sensorPos = 0; sensorPos < ULTRA_NUM_SENSORS; ++sensorPos) {
		sensors[static_cast<Common::UltrasonicPos>(sensorPos)].validate(currentSampleIndex);
		sensors[static_cast<Common::UltrasonicPos>(sensorPos)].updatePoint(currentSampleIndex);

		//Serial.print(sensors[sensorPos].dist_validated[currentSampleIndex]);
		//if (sensorPos != ULTRA_NUM_SENSORS - 1) Serial.print(", ");
	}

	//Serial.println();

	//Serial.print("stored: ");
	//Serial.print(storedDistances[Common::UltrasonicPos::FRONT_LEFT][currentSampleIndex]);
	//Serial.print("\t\t");
	//Serial.print("validated: ");
	//Serial.println(validatedDistances[Common::UltrasonicPos::FRONT_LEFT][currentSampleIndex]);
}

void Ultrasonic::updateSensorSelection() {

	uint8_t pos = static_cast<uint8_t>(currentSensorPos);

	// writes position to selection wires (as binary value)
	digitalWrite(ULTRA_SEL_0_PIN, (pos & 0x01) ? HIGH : LOW);
	digitalWrite(ULTRA_SEL_1_PIN, (pos & 0x02) ? HIGH : LOW);
	digitalWrite(ULTRA_SEL_2_PIN, (pos & 0x04) ? HIGH : LOW);
	digitalWrite(ULTRA_SEL_3_PIN, (pos & 0x08) ? HIGH : LOW);
}

void Ultrasonic::copyCurrentValidatedDistances(int dest[ULTRA_NUM_SENSORS]) const {
	noInterrupts();
	for (int sensorPos = 0; sensorPos < ULTRA_NUM_SENSORS; ++sensorPos)
		dest[sensorPos] = sensors[sensorPos].dist_validated[currentSampleIndex];
	interrupts();
}

bool Ultrasonic::cycleFinished() const {
	return currentSensorPos == ULTRA_NUM_SENSORS - 1;
}

void Ultrasonic::onWatchdoghasTimedOut() {
	//responsive[static_cast<int>(currentSensorPos)] = false;
	sensorConnection->timer_stop();

	/*Serial.print("not responsive: ");
	char s[1];
	Serial.println(itoa((int)currentSensorPos, s, 10));*/
	busy = false;
}

void Ultrasonic::Sensor::validate(int sampleIndex) {
	dist_stored[sampleIndex] = dist_measured;

	Common::ValidationData validationData = dist_measured == ULTRA_MAX_DISTANCE ?
		Ultrasonic::maxDistanceValidationData : Ultrasonic::defaultValidationData;

	int prevValidatedValue = dist_validated[(sampleIndex + ULTRA_NUM_DIST_SAMPLES - 1) % ULTRA_NUM_DIST_SAMPLES];
	int currentStoredValue = dist_stored[sampleIndex];

	/*
	Checks if current stored value is in the given range of the previous validated value for the sensor.
	e.g. if the previous validated value was 150, we will believe 145 without hesitation, no need for further validation
	*/
	if (Common::isInRange(prevValidatedValue, currentStoredValue, validationData.relativeError)) {
		dist_validated[sampleIndex] = currentStoredValue;
	} else {
		// if we got here, current stored value is out of range of the previous validated value so we need to validate

		int sampleCount = 0;
		bool outOfRangeFound = false;

		// iterates through stored values (from latest to earliest) and counts how many values are also out of range of previous validated value (sign of error is also important to match)
		// latest (validationSampleNum - 1) results should be out of range for us to believe that current value is valid
		for (int idx = (sampleIndex + ULTRA_NUM_DIST_SAMPLES - 1) % ULTRA_NUM_DIST_SAMPLES;
			!outOfRangeFound && (idx != sampleIndex);
			idx = (idx + ULTRA_NUM_DIST_SAMPLES - 1) % ULTRA_NUM_DIST_SAMPLES) {

			if (!Common::isInRange(prevValidatedValue, dist_stored[idx], validationData.relativeError,
				currentStoredValue < prevValidatedValue ? Common::ErrorSign::NEGATIVE : Common::ErrorSign::POSITIVE)) {
				++sampleCount;
			} else
				outOfRangeFound = true;
		}

		// validation OK -> we believe that distance is really what we measured
		// validation FAILED -> validated value will be previous validated value
		dist_validated[sampleIndex] = sampleCount >= validationData.validationSampleNum - 1 ?
			currentStoredValue : prevValidatedValue;
	}
}

void Ultrasonic::Sensor::updatePoint(int sampleIndex) {
	sensedPoint.X = pos.X + dist_validated[sampleIndex] * cos(viewAngle);
	sensedPoint.Y = pos.Y + dist_validated[sampleIndex] * sin(viewAngle);
}