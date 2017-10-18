#ifndef ULTRASONIC_TASK_HPP
#define ULTRASONIC_TASK_HPP

#include "PeriodicTask.hpp"
#include <NewPing.h>
#include "Point.hpp"

namespace rc_car {
	void ultrasonicEchoCheckIT();

	/** @brief Reads, validates and stores ultrasonic sensor data.
	*/
	class UltrasonicTask : public PeriodicTask {
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

		//Common::UltrasonicPos calculateForwardDirection(float steeringAngle);

	public:
		UltrasonicTask();

		void initialize();

		bool isBusy() const;

		bool isEnabled() const;

		void setEnabled(bool enabled);

		void pingNextSensor();

		void echoCheck();

		bool cycleFinished() const;

		void onWatchdoghasTimedOut();

		void validateAndUpdateSensedPoints();

		void getMeasuredPoints(Point<float> dest[ULTRA_NUM_SENSORS]);

		void __initialize() override;
		void __run() override;
		void __onTimedOut() override;

		~UltrasonicTask() {
			delete sensorConnection;
		}
	};
}

#endif	// ULTRASONIC_TASK_HPP
