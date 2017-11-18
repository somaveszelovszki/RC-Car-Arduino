#ifndef RC_CAR__ULTRASONIC_TASK__HPP
#define RC_CAR__ULTRASONIC_TASK__HPP

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

			Point2f pos;
			float viewAngle;
			Point2f sensedPoint;

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
		bool sendEnvironmentEnabled;

		Watchdog echoWatchdog;

		void updateSensorSelection();

		const Message::CODE ultraPosToMsgCode(Common::UltrasonicPos pos);

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

		const Point2f& getSensedPoint(Common::UltrasonicPos sensorPos) const;

		/** @brief Gets the UltrasonicPos that is nearest to the current steering angle.
		Therefore gives the direction of the car in UltrasonicPos.
		*/
		Common::UltrasonicPos getForwardPos(float steeringAngle) const;

		float getSensorViewAngle(Common::UltrasonicPos sensorPos) const;

		void executeMessage();
	};
}

#endif // RC_CAR__ULTRASONIC_TASK__HPP
