#ifndef RC_CAR__TRAJECTORY__HPP
#define RC_CAR__TRAJECTORY__HPP

#include "CarProps.hpp"
#include "Periodic.hpp"

namespace rc_car {
/** @brief Calculates car trajectory from steering angle, and calculates object distance from this trajectory.
*/
class Trajectory : public Periodic {
public:
    /** @brief Stores information about the distance of an obstacle and the car track.
    */
    class TrackDistance {
    public:
        /** @brief Distance between the obstacle and the car track (the side of the car).
        If distance is negative, then is is in the track - the car will hit it eventually.
        */
        float dist;

        /** @brief Direction of the obstacle relative to the car track middle.
        Helps determining the steering direction in which it is easier to avoid the crash.
        */
        Common::RotationDir dir;

        /** @brief Remaining time before the car reaches the point of the trajectory where it is nearest to (or where it hits) the obstacle.
        */
        float remainingTime;

        /** @brief Checks if The distance between the obstacle and the track is critical
        (if car is going to hit the obstacle, or go too close to it).

        @returns Boolean value indicating if the distance is critical.
        */
        bool isCritical() const {
            return dist <= MIN_OBSTACLE_DIST;
        }
    };

private:
    /** @brief Pointer to the structure storing the car properties (speed, steering angle, etc).
    */
    CarProps *pCar;

    /** @brief The current radius of the rear middle point of the car.
    */
    float R_rearMid;

    /** @brief The current radius of the most inner point of the car.
    */
    float R_inner;

    /** @brief The current radius of the most outer point of the car.
    */
    float R_outer;

    /** @brief The current radius of the near front point of the car.
    */
    float R_frontNear;

    /** @brief The current radius of the far rear point of the car.
    */
    float R_rearFar;

    /** @brief Indicates if steering angle is so small that it is not worth calculating with,
    but calculations can be done with 0.
    */
    bool isNoSteering;

    /** @brief Updates radiuses according to the steering angle.
    */
    void updateRadiuses();

public:
    /** @brief Constructor - sets period time, initializes position and angle.

    @param _periodTime The period time.
    @param _pCar Pointer to the structure storing the car properties.
    */
    Trajectory(int _periodTime, CarProps *_pCar) : Periodic(_periodTime) {}

    /** @brief Updates speed, steering angle and then updates radiuses according to the steering angle.

    @param _speed The current speed of the car.
    @param _steeringAngle The current steering angle.
    */
    void update(float _speed, float _steeringAngle);

    /** @brief Calculates distance between given point and the car trajectory.

    @param relativePos Position of the obstacle relative to the car.
    @param forceCalcRemainingTime Indicates if remaining time should be calculated even if car will not hit the obstacle.
    @returns A TrackDistance object storing information about the distance between the obstacle and the trajectory.
    */
    TrackDistance trackdistancePoint(const Point2f& relativePos, bool forceCalcRemainingTime = false) const;
};
}

#endif // RC_CAR__TRAJECTORY__HPP
