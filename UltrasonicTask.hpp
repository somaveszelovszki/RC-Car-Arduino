#ifndef RC_CAR__ULTRASONIC_TASK__HPP
#define RC_CAR__ULTRASONIC_TASK__HPP

#include "PeriodicTask.hpp"
#include <NewPing.h>
#include "Environment.hpp"
#include "Message.hpp"

namespace rc_car {
    /** @brief Callback function for checking ECHO signal. Called by NewPing library in every 24us while waiting for the ECHO.
    Calls the global UltrasonicTask's echoCheck() function, which checks if the ECHO signal has arrived, and if yes, gets distance.
    */
	void ultrasonicEchoCheckIT();

	/** @brief Reads, validates and stores ultrasonic sensor data.
	*/
	class UltrasonicTask : public PeriodicTask {
	private:
        /** @brief Contains ultrasonic sensor data.
        */
		class Sensor {
		public:
            /** @brief Index of the sensor in the circuit.
            */
			int selIdx;

            /** @brief Current measured distance.
            */
			int dist_measured;

            /** @brief Array of stored distances.
            */
			int dist_stored[ULTRA_NUM_DIST_SAMPLES];

            /** @brief Current validated distance.
            */
			int dist_valid;

            /** @brief The sensor's position - relative to the car's current position.
            */
			Point2f pos;

            /** @brief The sensor's view angle relative to the car's forward direction - in [rad].
            */
			float viewAngle;

            /** @brief The sensed points position - relative to the car's current position.
            */
			Point2f sensedPoint;

            /** @brief Validates measured points for all the sensors.

            @param sampleIndex The index of the sample that needs to be validated.
            NOTE: Always equals currentSampleIndex!
            */
			void validate(int sampleIndex);

            /** @brief Updates sensed point according to sensor's position, sensor's view angle and the current validated distance.
            */
			void updatePoint();
		};

        /** @brief Array of sensors containing sensor data and distances.
        */
		Sensor sensors[ULTRA_NUM_SENSORS];

        /** @brief Sensor connection makes the NewPing functions available.
        */
		NewPing sensorConnection;

        /** @brief Validation object for maximum distance.
        When a sensor loses the signal, maximum distance will be returned by the NewPing library.
        As this often happens, max distance validation differs from default validation, that's why it has its own validation object.
        */
		static const Common::Validation MAX_DIST_VALIDATION;

        /** @brief Validation object for all distances.
        */
		static const Common::Validation DEF_VALIDATION;

        /** @brief Used for iterating through all sensors.
        */
		Common::UltrasonicPos currentSensorPos;

        /** @brief Indicates if sensor connection is busy.
        NOTE: Next sensor should only be pinged when the busy flag is not set!
        */
		bool busy;

        /** @brief Indicates if ultrasonic distance measuring is enabled.
        */
		bool enabled;

        /** @brief Index of current sample index in the stored distances array.
        */
		int currentSampleIndex;

        /** @brief Used for storing messages acquired from the CommunicatorTask.
        */
		Message msg;

        /** @brief Indicates if environment point sending is enabled.
        */
		bool sendEnvironmentEnabled;

        /** @brief Watchdog for ECHO timeout detection.
        */
		Watchdog echoWatchdog;

        /** @brief Updates sensor selection - sets selection pins.
        */
		void updateSensorSelection();

        /** @brief Converts ultrasonic sensor position to message code.

        @param pos The ultrasonic sensor position.
        @returns The message code.
        */
        Message::CODE ultraPosToMsgCode(Common::UltrasonicPos pos) {
            return static_cast<Message::CODE>(static_cast<int>(Message::CODE::Ultra0_1_EnvPoint) + static_cast<int>(pos) / 2);
        }

	public:
        /** @brief Constructor - initializes period time, timeout and sensors' data, sets up sensor connection.
        */
		UltrasonicTask();

        /** @brief Sets pin mode for selection pins, resets sensors' distances.
        NOTE: Compulsory TASK function - initializes task!
        */
        void initialize();

        /** @brief Gets message from the CommunicatorTask,
        if previous sensor's distance has been read, pings next sensor,
        if all sensors' distance has been read, validates these distances,
        if point sending is enabled, sends points.
        NOTE: Compulsory TASK function - called in every cycle!
        */
        void run();

        /** @brief Restarts timeout check.
        NOTE: Compulsory TASK function - called when task watchdog timed out!
        */
        void onTimedOut() {
            restartTimeoutCheck();
        }

        /** @brief Checks if sensor connection is busy.

        @returns Boolean value indicating if sensor connection is busy.
        */
        bool isBusy() const {
            return busy;
        }

        /** @brief Checks if ultrasonic distance measuring is enabled.

        @returns Boolean value indicating if ultrasonic distance measuring is enabled.
        */
        bool isEnabled() const {
            return enabled;
        }

        /** @brief Enables/disables ultrasonic distance measuring.

        @param enabled Boolean value indicating if ultrasonic distance measuring is enabled.
        */
        void setEnabled(bool enabled) {
            this->enabled = enabled;
        }

        /** @brief Updates sensor position, and sends TRIGGER signal to the ultrasonic sensor.
        */
		void pingNextSensor();

        /** @brief Checks if ECHO signal has arrived or if it has timed out, and sets sensor distance.
        */
		void echoCheck();

        /** @brief Checks if measurement cycle has finished.

        @returns Boolean values indicating if measurement cycle has finished.
        */
        bool measurementCycleFinished() const {
            return static_cast<int>(currentSensorPos) == ULTRA_NUM_SENSORS - 1;
        }

        /** @brief Validates distances for all sensors and updates sensed points according to these validated distances.
        TODO: instead of this, validate and update instantly after every sensor (before next ping).
        */
		void validateAndUpdateSensedPoints();

        /** @brief Gets sensed point of the selected sensor.

        @param sensorPos The position of the sensor.
        @returns The sensed point of the selected sensor.
        */
        const Point2f& getSensedPoint(Common::UltrasonicPos sensorPos) const {
            return sensors[static_cast<int>(sensorPos)].sensedPoint;
        }
		/** @brief Gets the UltrasonicPos that is nearest to the current steering angle.
		Therefore gives the direction of the car in UltrasonicPos.

        @param steeringAngle The current steering angle.
        @returns The UltrasonicPos nearest to the steering angle.
		*/
		Common::UltrasonicPos getForwardPos(float steeringAngle) const;

        /** @brief Gets view angle of selected sensor.

        @param sensorPos The position of the sensor.
        @returns The view angle of selected sensor.
        */
        float getSensorViewAngle(Common::UltrasonicPos sensorPos) const {
            return sensors[static_cast<int>(sensorPos)].viewAngle * RAD_TO_DEG;
        }

        /** @brief Executes message that has previously been acquired from the CommunicatorTask.
        */
		void executeMessage();
	};
}

#endif // RC_CAR__ULTRASONIC_TASK__HPP
