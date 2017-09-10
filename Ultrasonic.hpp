#ifndef ULTRA_HPP
#define ULTRA_HPP

#include <NewPing.h>
#include "Watchdog.hpp"
#include "Point.hpp"

class Ultrasonic {

private:

	class Sensor {
	public:
		int dist_measured;
		int dist_stored[ULTRA_NUM_DIST_SAMPLES];
		int dist_validated[ULTRA_NUM_DIST_SAMPLES];
		int nonResponsiveCounter;

		Point<float> pos;
		float viewAngle;
		Point<float> sensedPoint;

		void validate(int sampleIndex);

		void updatePoint(int sampleIndex);
	};

	Sensor sensors[ULTRA_NUM_SENSORS];

	NewPing *sensorConnection;

	static const Common::ValidationData maxDistanceValidationData;
	static const Common::ValidationData defaultValidationData;

	Common::UltrasonicPos currentSensorPos;
	bool busy;

	bool enabled;

	int currentSampleIndex;

	void updateSensorSelection();

public:

	Ultrasonic();

	void initialize();

	bool isBusy() const;

	bool isEnabled() const;

	void setEnabled(bool enabled);

	void pingNextSensor();

	void echoCheck();

	bool cycleFinished() const;

	void onWatchdoghasTimedOut();

	void validateAndUpdateSensedPoints();

	void copyCurrentValidatedDistances(int dest[ULTRA_NUM_SENSORS]) const;

	~Ultrasonic() {
		delete sensorConnection;
	}

};

#endif	// ULTRA_HPP

