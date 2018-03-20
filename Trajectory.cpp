#include "Trajectory.hpp"

using namespace rc_car;

void Trajectory::updateRadiuses() {
    float cosAngle = cos(this->pCar->steeringAngle);
    if (!(isNoSteering = Common::eq(cosAngle, 0.0f))) {
        float sinAngle = sin(this->pCar->steeringAngle), tanAngle = sinAngle / cosAngle;
        int steerMul = static_cast<int>(this->pCar->steeringDir);

        R_rearMid = CAR_PIVOT_DIST_FRONT_REAR / tanAngle;

        R_outer = Common::pythagoras(R_rearMid + CAR_PIVOT_LENGTH * steerMul,
            CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_FRONT_DIST) * steerMul;

        R_inner = R_rearMid - CAR_PIVOT_LENGTH * steerMul;

        R_frontNear = Common::pythagoras(R_inner, CAR_PIVOT_DIST_FRONT_REAR) * steerMul;
        R_rearFar = R_rearMid + CAR_PIVOT_LENGTH * steerMul;
    }
}

void Trajectory::update(float _speed, float _steeringAngle) {
    this->pCar->steeringAngle = _steeringAngle;
    this->pCar->steeringDir = this->pCar->steeringAngle >= 0.0f ? Common::RotationDir::LEFT : Common::RotationDir::RIGHT;
    this->pCar->speed = _speed;

    updateRadiuses();
}

//void Trajectory::updateRadiuses() {
//    if (!(isNoSteering = (abs(this->pCar->steeringAngle) <= 0.05f))) {
//        float tanAngle = tanf(this->pCar->steeringAngle);
//
//        int steerMul = static_cast<int>(this->pCar->steeringDir);
//
//        R_rearMid = CAR_PIVOT_DIST_FRONT_REAR / tanAngle;
//
//        R_outer = pythag(R_rearMid + CAR_PIVOT_LENGTH * steerMul, CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_FRONT_DIST) * steerMul;
//        R_inner = R_rearMid - CAR_PIVOT_LENGTH * steerMul;
//        R_rearFar = R_rearMid + CAR_PIVOT_LENGTH * steerMul;
//        R_frontNear = pythag(R_inner, CAR_PIVOT_DIST_FRONT_REAR) * steerMul;
//    }
//}
//
//void Trajectory::update(float _speed, float _steeringAngle) {
//    unsigned long d_time = millis() - getPrevCalledTime();
//
//    this->pCar->speed = _speed;
//
//    if (this->pCar->steeringAngle != _steeringAngle) {
//        this->pCar->steeringAngle = _steeringAngle;
//        this->pCar->steeringDir = _steeringAngle >= 0.0f ? Common::RotationDir::LEFT : Common::RotationDir::RIGHT;
//        updateRadiuses();
//    }
//
//    if (!isNoSteering) {
//        this->pCar->fwdAngle += d_time * this->pCar->speed / R_rearFar;
//        this->pCar->fwdAngle_Cos = cosf(this->pCar->fwdAngle);
//        this->pCar->fwdAngle_Sin = sinf(this->pCar->fwdAngle)/*sqrtf(1 - this->pCar->fwdAngle_Cos * this->pCar->fwdAngle_Cos)*/;
//    }
//
//    this->pCar->pos.X += this->pCar->speed * this->pCar->fwdAngle_Cos * d_time;
//    this->pCar->pos.Y += this->pCar->speed * this->pCar->fwdAngle_Sin * d_time;
//}

Trajectory::TrackDistance Trajectory::trackdistancePoint(const Point2f& relativePos, bool forceCalcRemainingTime) const {
    TrackDistance td;

    if (isNoSteering) {
        float innerDist = abs(-CAR_PIVOT_LENGTH - relativePos.X),
            outerDist = abs(CAR_PIVOT_LENGTH - relativePos.X);

        // checks if car hits obstacle - if yes, minimum distance is negative
        td.dist = (Common::isBtw(relativePos.X, -CAR_PIVOT_LENGTH, CAR_PIVOT_LENGTH) ? -1 : 1) * min(innerDist, outerDist);
        td.dir = innerDist <= outerDist ? Common::RotationDir::LEFT : Common::RotationDir::RIGHT;

        // calculates remaining time
        if (td.isCritical() || forceCalcRemainingTime)
            td.remainingTime = (relativePos.Y - (CAR_HEIGHT / 2) * (relativePos.Y > 0 ? 1 : -1)) / this->pCar->speed;
    } else {
        // origo    center of the trajectory circle of the rear pivot's center
        // obs        position of the obstacle relative to the rear pivot's center
        Point2f origo(-R_rearMid, 0.0), obs;

        obs.X = relativePos.X + R_rearMid;
        obs.Y = relativePos.Y + CAR_PIVOT_DIST_MID;

        float obsAngle = origo.getAngle(obs, this->pCar->steeringDir);

        float abs_R_inner = abs(R_inner), abs_R_outer = abs(R_outer);

        float obsDist = obs.distance(origo),
            innerDist = abs(obsDist - abs_R_inner),
            outerDist = abs(obsDist - abs_R_outer);

        // checks if car hits obstacle - if yes, minimum distance is negative
        td.dist = (Common::isBtw(obsDist, abs_R_inner, abs_R_outer) ? -1 : 1) * min(innerDist, outerDist);
        td.dir = static_cast<Common::RotationDir>((innerDist < outerDist ? 1 : -1) * static_cast<int>(this->pCar->steeringDir));

        // calculates remaining time
        if (td.isCritical() || forceCalcRemainingTime) {

            // delta angle on the trajectory
            // -> the given part of the car that hits the obstacle will reach it before the rear pivot does
            // this angle specifies this difference
            float dAngle = 0.0;

            if (td.isCritical()) {
                if (innerDist < min(outerDist, abs(R_frontNear - R_inner)))
                    dAngle = atanf(CAR_PIVOT_DIST_FRONT_REAR / R_inner) * (innerDist / abs(R_frontNear - R_inner));
                else
                    dAngle = atanf(CAR_PIVOT_DIST_FRONT_REAR / R_inner);
            }

            float hitAngle = obsAngle - dAngle;
            td.remainingTime = R_rearFar * hitAngle / this->pCar->speed;
        }
    }

    return td;
}
