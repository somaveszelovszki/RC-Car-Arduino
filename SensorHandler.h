#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "Watchdog.h"
#include "PeriodicThread.h"
#include <NewPing.h>

#define ULTRASONIC_FRONT_LEFT_TRIG_PIN A0
#define ULTRASONIC_FRONT_LEFT_ECHO_PIN A1

#define ULTRASONIC_FRONT_RIGHT_TRIG_PIN A2
#define ULTRASONIC_FRONT_RIGHT_ECHO_PIN A3

#define ULTRASONIC_BACK_LEFT_TRIG_PIN 5
#define ULTRASONIC_BACK_LEFT_ECHO_PIN 6

#define ULTRASONIC_BACK_RIGHT_TRIG_PIN 6
#define ULTRASONIC_BACK_RIGHT_ECHO_PIN 13
// TODO BACK RIGHT not working yet

#define ULTRASONIC_NUM_SENSORS 4
#define ULTRASONIC_NUM_DISTANCE_SAMPLES 10
#define ULTRASONIC_MAX_DISTANCE 200

/*
	Reads, validates and stores sensors' data.
*/
class SensorHandler : public PeriodicThread {

private:
	static const unsigned long DEFAULT_CYCLE_PERIOD = 1;

public:

	class Ultrasonic {

	public:

		class UltrasonicSensor : public NewPing {

		public:
			UltrasonicSensor(uint8_t trigger_pin, uint8_t echo_pin);
		};

	private:

		UltrasonicSensor sensors[ULTRASONIC_NUM_SENSORS] = {     // Sensor object array.
			UltrasonicSensor(ULTRASONIC_FRONT_LEFT_TRIG_PIN, ULTRASONIC_FRONT_LEFT_ECHO_PIN),	// front left
			UltrasonicSensor(ULTRASONIC_FRONT_RIGHT_TRIG_PIN, ULTRASONIC_FRONT_RIGHT_ECHO_PIN),	// front right
			UltrasonicSensor(ULTRASONIC_BACK_LEFT_TRIG_PIN, ULTRASONIC_BACK_LEFT_ECHO_PIN),		// back left
			UltrasonicSensor(ULTRASONIC_BACK_RIGHT_TRIG_PIN, ULTRASONIC_BACK_RIGHT_ECHO_PIN)	// back right
		};

		Common::POSITION currentSensorPos;
		bool busy;
		unsigned long measuredDistances[ULTRASONIC_NUM_SENSORS];

		Watchdog* watchdog = new Watchdog(10);	// timeout in ms

		// marks unresponsive sensors, that do not have to be pinged
		bool responsive[ULTRASONIC_NUM_SENSORS];

		bool enabled;

		unsigned long storedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned long validatedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned int currentSampleIndex;

		void updateDistances(Common::POSITION sensorPos);

	public:

		Ultrasonic();

		void initialize();

		bool isBusy();

		bool isEnabled();

		void setEnabled(bool enabled);

		void pingNextSensor();

		void echoCheck();

		bool cycleFinished();

		Watchdog* getWatchdog() const;

		void onWatchdogTimedOut();

		void updateDistances();

		void copyCurrentValidatedDistances(unsigned long dest[ULTRASONIC_NUM_SENSORS]);

	};

public:
	Ultrasonic* ultrasonic;

	SensorHandler();

	void initialize();
};

void ultrasonicEchoCheckIT();

#endif
