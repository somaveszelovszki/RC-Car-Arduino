#ifndef ULTRASONIC_TASK_HPP
#define ULTRASONIC_TASK_HPP

#include "PeriodicTask.hpp"
#include <NewPing.h>
#include "Environment.hpp"
#include "Message.hpp"

namespace rc_car {
	void ultrasonicEchoCheckIT();

	/** @brief Reads, validates and stores ultrasonic sensor data.
	*/
	class UltrasonicTask : public PeriodicTask {
	private:
		class Sensor {
		public:
			int selIdx;
			int dist_measured;
			int dist_stored[ULTRA_NUM_DIST_SAMPLES];
			int dist_valid;
			int nonResponsiveCounter;

			Point<float> pos;
			float viewAngle;
			Point<float> sensedPoint;

			void validate(int sampleIndex);

			void updatePoint();
		};

		Sensor sensors[ULTRA_NUM_SENSORS];

		NewPing sensorConnection;

		static const Common::Validation MAX_DIST_VALIDATION;
		static const Common::Validation DEF_VALIDATION;

		Common::UltrasonicPos currentSensorPos;
		bool busy;

		bool enabled;
		int currentSampleIndex;

		Message msg;
		Environment environment;
		bool sendEnvironmentEnabled = false;

		void updateSensorSelection();

		const Message::CODE ultraPosToMsgCode(Common::UltrasonicPos pos);

		//Common::UltrasonicPos calculateForwardDirection(float steeringAngle);

	public:
		UltrasonicTask();

		void initialize();
		void run();
		void onTimedOut();

		bool isBusy() const;

		bool isEnabled() const;

		void setEnabled(bool enabled);

		void pingNextSensor();

		void echoCheck();

		bool measurementCycleFinished() const;

		void validateAndUpdateSensedPoints();

		void getMeasuredPoints(Point<float> dest[ULTRA_NUM_SENSORS]);

		void executeMessage();
	};
}

#endif	// ULTRASONIC_TASK_HPP
