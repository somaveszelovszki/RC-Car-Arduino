#ifndef RC_CAR__CAR_PROPS__HPP
#define RC_CAR__CAR_PROPS__HPP

#include "Point2.hpp"

namespace rc_car {
class CarProps {
public:
    /** @brief Position of the car relative to its start position.
    */
    Point2f pos;

    /** @brief The current speed of the car.
    */
    float speed;

    /** @brief The current steering angle.
    NOTE: Steering angle is relative to the Y axis!
    */
    float steeringAngle;

    /** @brief The current steering direction.
    */
    Common::RotationDir steeringDir;

    /** @brief Car's forward angle in [rad].
    NOTE: Steering angle is relative to the X axis!
    */
    float fwdAngle;

    /** @brief Car's forward angle's cosine.
    NOTE: Steering angle is relative to the X axis!
    */
    float fwdAngle_Cos;

    /** @brief Car's forward angle's sine.
    NOTE: Steering angle is relative to the X axis!
    */
    float fwdAngle_Sin;

    CarProps() :
        pos(0.0f, 0.0f),
        speed(0.0f),
        steeringAngle(0.0f),
        fwdAngle(M_PI_2),
        fwdAngle_Cos(0.0f),
        fwdAngle_Sin(1.0f) {}
};
}

#endif // RC_CAR__CAR_PROPS__HPP