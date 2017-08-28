#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP

#include <NewPing.h>
#include "Watchdog.hpp"

class Ultrasonic {

public:

	/*
		NOTE: If you add a position, don't forget to update ULTRASONIC_NUM_SENSORS!
	*/
	enum POSITION {
		FRONT_LEFT_CORNER = 0,
		FRONT_LEFT = 1,
		FRONT_RIGHT = 2,
		FRONT_RIGHT_CORNER = 3,
		REAR_LEFT_CORNER = 4,
		REAR_LEFT = 5,
		REAR_RIGHT = 6,
		REAR_RIGHT_CORNER = 7,
		LEFT_FRONT = 8,
		LEFT_REAR = 9,
		RIGHT_FRONT = 10,
		RIGHT_REAR = 11
	};

private:

	NewPing *sensorConnection = new NewPing(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_MAX_DISTANCE);

	static const Common::ValidationData maxDistanceValidationData;
	static const Common::ValidationData defaultValidationData;

	POSITION currentSensorPos;
	bool busy;
	int measuredDistances[ULTRASONIC_NUM_SENSORS];
	
	// marks unresponsive sensors, that do not have to be pinged
	bool responsive[ULTRASONIC_NUM_SENSORS];

	bool enabled;

	int storedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
	int validatedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
	int currentSampleIndex;

	void updateDistances(POSITION sensorPos);

	void validate(POSITION sensorPos, Common::ValidationData validationData);

	void updateSensorSelection();

public:

	Ultrasonic();

	void initialize();

	bool isBusy();

	bool isEnabled();

	void setEnabled(bool enabled);

	void pingNextSensor();

	void echoCheck();

	bool cycleFinished();

	void onWatchdogTimedOut();

	void updateDistances();

	void copyCurrentValidatedDistances(int dest[ULTRASONIC_NUM_SENSORS]);

};

#endif	// ULTRASONIC_HPP

