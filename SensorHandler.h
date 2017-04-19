#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "Watchdog.h"
#include "PeriodicThread.h"
#include <NewPing.h>

#define ULTRASONIC_FRONT_LEFT_TRIG_PIN A0
#define ULTRASONIC_FRONT_LEFT_ECHO_PIN A1

#define ULTRASONIC_FRONT_RIGHT_TRIG_PIN A2
#define ULTRASONIC_FRONT_RIGHT_ECHO_PIN A3

#define ULTRASONIC_BACK_LEFT_TRIG_PIN 13
#define ULTRASONIC_BACK_LEFT_ECHO_PIN 13

#define ULTRASONIC_BACK_RIGHT_TRIG_PIN 13
#define ULTRASONIC_BACK_RIGHT_ECHO_PIN 13
// TODO BACK RIGHT not working yet

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

#define ROTARY_ENCODER_A_PIN 2
#define ROTARY_ENCODER_B_PIN 3

#define ROTARY_RESOLUTION 20

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

		class UltrasonicSensor : public NewPing {

		public:
			UltrasonicSensor(uint8_t trigger_pin, uint8_t echo_pin);
		};

	private:

		static const ValidationData maxDistanceValidationData;
		static const ValidationData defaultValidationData;


		UltrasonicSensor sensors[ULTRASONIC_NUM_SENSORS] = {     // Sensor object array.
			UltrasonicSensor(ULTRASONIC_FRONT_LEFT_TRIG_PIN, ULTRASONIC_FRONT_LEFT_ECHO_PIN),	// front left
			UltrasonicSensor(ULTRASONIC_FRONT_RIGHT_TRIG_PIN, ULTRASONIC_FRONT_RIGHT_ECHO_PIN),	// front right
			UltrasonicSensor(ULTRASONIC_BACK_LEFT_TRIG_PIN, ULTRASONIC_BACK_LEFT_ECHO_PIN),		// back left
			UltrasonicSensor(ULTRASONIC_BACK_RIGHT_TRIG_PIN, ULTRASONIC_BACK_RIGHT_ECHO_PIN)	// back right
		};

		Common::POSITION currentSensorPos;
		bool busy;
		unsigned long measuredDistances[ULTRASONIC_NUM_SENSORS];

		Watchdog *watchdog = new Watchdog(10);	// timeout in ms

		// marks unresponsive sensors, that do not have to be pinged
		bool responsive[ULTRASONIC_NUM_SENSORS];

		bool enabled;

		unsigned long storedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned long validatedDistances[ULTRASONIC_NUM_SENSORS][ULTRASONIC_NUM_DISTANCE_SAMPLES];
		unsigned int currentSampleIndex;

		void updateDistances(Common::POSITION sensorPos);

		void validate(Common::POSITION sensorPos, ValidationData validationData);

		bool isInRange(unsigned long ref, unsigned long value, unsigned int errorPercent, Common::ERROR_SIGN errorDir);

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

	class RotaryEncoder {
	public:

		struct Result {
			uint64_t d_time;	// [ms]
			int d_pos;
		};

	private:
		uint8_t A_pin;
		uint8_t B_pin;

		bool A_set = false;
		bool B_set = false;

		int position;
		uint64_t time;

		Result storedResult;

		Watchdog *watchdog = new Watchdog(200);	// timeout in ms

	public:
		RotaryEncoder(uint8_t A_pin, uint8_t B_pin);

		void initialize();

		void onChange_A();
		void onChange_B();

		Result readAndUpdateIfTimedOut();

		Watchdog *getWatchdog();
	};

public:
	Ultrasonic *ultrasonic;
	RotaryEncoder *rotaryEncoder;

	SensorHandler();

	void initialize();

	void watchdogDecrement();
};

void doRotaryEncoderA();

void doRotaryEncoderB();

void ultrasonicEchoCheckIT();

#endif
