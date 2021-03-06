#ifndef RC_CAR__DRIVE_TASK__HPP
#define RC_CAR__DRIVE_TASK__HPP

#include "CarProps.hpp"
#include "MotorHandler.hpp"
#include "Trajectory.hpp"
#include "Environment.hpp"

namespace rc_car {
/** @brief Makes driving decisions from sensor data and car state. Controls motors with the MotorHandler.
*/
class DriveTask : public PeriodicTask {

private:
    /** @brief Drive mode determines how much influence the firmware has got in making driving decisions.
    */
    Common::DriveMode mode;

    /** @ brief Stores car properties (speed, steering angle, etc).
    */
    CarProps car;

    /** @brief Handles the DC and servo motors.
    */
    MotorHandler motorHandler;

    /** @brief Watchdog for message receiving.
    NOTE: If no messages arrive in a given period of time (so the connection is probably broken), the car has to be stopped!
    */
    Watchdog msgWatchdog;

    /** @brief Watchog for force steering - counts time while user steering commands need to be ignored.
    */
    Watchdog forceSteeringWatchdog;

    /** @brief Watchog for force stop - counts time while user speed commands need to be ignored.
    */
    Watchdog forceStopWatchdog;

    /** @brief Indicates if environment grid sending is enabled.
    */
    bool sendEnvGridEnabled;

    /** @brief Watchdog for environment grid update.
    */
    Watchdog envGridUpdateWatchdog;

    enum EnvGridSendState {
        RESET = 0, CAR, COORDS
    };

    EnvGridSendState envGridSendState;

    /** @brief Calculates environment points according to sensed points received from the UltrasonicTask.
    */
    Environment environment;

    /** @brief Calculates car trajectory accroding to steering angle
    and if an obstacle is found, calculates remaining time until the crash.
    */
    Trajectory trajectory;

    /** @brief Will store the received message whenever there is a new one.
    */
    Message msg;

    /** @brief Remaining time until crash for each section.
    */
    float remainingTimes[ENV_SECTIONS_NUM];

    /** @brief Indicates if force steering is active.
    */
    bool forceSteeringActive = false;

    /** @brief Indicates if force stop is active.
    */
    bool forceStopActive = false;

    /** @brief Executes message that has previously been acquired from the CommunicatorTask.
    */
    void executeMessage();

    /** @brief Checks if given distance is critical.

    TODO: Remove this function, find a more elegant way of deciding if command override is needed!

    @param dist The distance.
    @param minDist The minimum distance to compare the distance to.
    @returns Boolean value indicating if the distance is critical.
    */
    static bool isDistanceCritical(float dist, float minDist);

    void calculateRemainingTimes(Common::UltrasonicPos startPos);

public:
    /** @brief Constructor - sets watchdog timeouts.
    */
    DriveTask();

    /** @brief Initializes motor handler.
    NOTE: Compulsory TASK function - initializes task!
    */
    void initialize();

    /** @brief Updates speed and steering angle according to received message, drive mode and environment points.
    NOTE: Compulsory TASK function - called in every cycle!
    */
    void run();

    /** @brief Restarts timeout check.
    NOTE: Compulsory TASK function - called when task watchdog timed out!
    */
    void onTimedOut() {
        restartTimeoutCheck();
    }
};

}
#endif // RC_CAR__DRIVE_TASK__HPP

