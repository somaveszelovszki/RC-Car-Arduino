#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "Watchdog.h"
#include "PeriodicThread.h"
#include <NewPing.h>

#define ULTRASONIC_TRIGGER_PIN A4
#define ULTRASONIC_ECHO_PIN ULTRASONIC_TRIGGER_PIN

#define ULTRASONIC_SEL_0 A0
#define ULTRASONIC_SEL_1 A1
#define ULTRASONIC_SEL_2 A2
#define ULTRASONIC_SEL_3 A3

#define ULTRASONIC_NUM_SENSORS 4
#define ULTRASONIC_NUM_DISTANCE_SAMPLES 10
#define ULTRASONIC_MAX_DISTANCE 200

// defines how many stored measured values need to be ULTRASONIC_MAX_DISTANCE, so that we can validate value
//		->	many times the sensors do not respond, which equals ULTRASONIC_MAX_DISTANCE as a value,
//			so we must not believe it unless the result is the same for a few times in a row
#define maxDistanceValidationSampleNum 5		// -> if the result has been ULTRASONIC_MAX_DISTANCE for 2 times in a row, we believe it
#define maxDistanceValidationErrorPercent 0
#define defaultValidationSampleNum 2
#define defaultValidationErrorPercent 50

/*
	Reads, validates and stores sensors' data.
*/
class SensorHandler : public PeriodicThread {

private:
	static const unsigned long DEFAULT_CYCLE_PERIOD = 1;

public:

	struct ValidationData {
		unsigned int validationSampleNum;
		unsigned int errorPercent;
	};

	class Ultrasonic {

	public:

		/*
		If you add a position, update POSITION_COUNT
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

		static const ValidationData maxDistanceValidationData;
		static const ValidationData defaultValidationData;

		POSITION currentSensorPos;
		bool busy;
		unsigned long measuredDistances[ULTRASONIC_NUM_SENSORS];

		Watchdog *watchdog = new Watchdog(10);	// timeout in ms

		// marks unresponsive sensors, that do not have to be pinged
		bool responsive[ULTRASONIC_NUM_SENSORS];
		
		bool enabled;

		unsigned long storedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned long validatedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned int currentSampleIndex;

		void updateDistances(POSITION sensorPos);

		void validate(POSITION sensorPos, ValidationData validationData);

		bool isInRange(unsigned long ref, unsigned long value, unsigned int errorPercent, Common::ERROR_SIGN errorDir);

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

		Watchdog *getWatchdog() const;

		void onWatchdogTimedOut();

		void updateDistances();

		void copyCurrentValidatedDistances(unsigned long dest[ULTRASONIC_NUM_SENSORS]);

	};

public:
	Ultrasonic *ultrasonic;

	SensorHandler();

	void initialize();

	void watchdogDecrement();
};

void ultrasonicEchoCheckIT();

#endif
