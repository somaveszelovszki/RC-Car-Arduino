#include "Trajectory.hpp"

using namespace rc_car;

void Trajectory::updateRadiuses() {
    if (!(isNoSteering = (abs(pCar->steeringAngle) <= 0.05f))) {
        float tanAngle = tanf(pCar->steeringAngle);

        int steerMul = static_cast<int>(pCar->steeringDir);

        R_rearMid = CAR_PIVOT_DIST_FRONT_REAR / tanAngle;

        R_outer = Common::pythag(R_rearMid + CAR_PIVOT_LENGTH * steerMul, CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_FRONT_DIST) * steerMul;
        R_inner = R_rearMid - CAR_PIVOT_LENGTH * steerMul;
        R_rearFar = R_rearMid + CAR_PIVOT_LENGTH * steerMul;
        R_frontNear = Common::pythag(R_inner, CAR_PIVOT_DIST_FRONT_REAR) * steerMul;
    } else
        R_rearMid = R_outer = R_inner = R_rearFar = R_frontNear = 0.0f;
}

void Trajectory::update(float _speed, float _steeringAngle) {
    float d_time = (millis() - getPrevCalledTime()) / 1000.0f; // in [sec]

    pCar->speed = _speed;

    if (pCar->steeringAngle != _steeringAngle) {
        pCar->steeringAngle = _steeringAngle;
        pCar->steeringDir = _steeringAngle >= 0.0f ? Common::RotationDir::LEFT : Common::RotationDir::RIGHT;
        updateRadiuses();
    }

    if (!isNoSteering) {
        pCar->fwdAngle += d_time * pCar->speed / R_rearFar;

        // normalizes angle to the [0, 2*PI) interval
        if (pCar->fwdAngle >= 2 * M_PI)
            pCar->fwdAngle -= 2 * M_PI;

        if (pCar->fwdAngle < 0)
            pCar->fwdAngle += 2 * M_PI;

        pCar->fwdAngle_Cos = cosf(pCar->fwdAngle);
        pCar->fwdAngle_Sin = sinf(pCar->fwdAngle);
    }

    pCar->pos.X += pCar->speed * pCar->fwdAngle_Cos * d_time;
    pCar->pos.Y += pCar->speed * pCar->fwdAngle_Sin * d_time;
}

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
            td.remainingTime = (relativePos.Y - (CAR_HEIGHT / 2) * (relativePos.Y > 0 ? 1 : -1)) / pCar->speed;
    } else {
        // origo    center of the trajectory circle of the rear pivot's center
        // obs        position of the obstacle relative to the rear pivot's center
        Point2f origo(-R_rearMid, 0.0), obs;

        obs.X = relativePos.X + R_rearMid;
        obs.Y = relativePos.Y + CAR_PIVOT_DIST_MID;

        float obsAngle = origo.getAngle(obs, pCar->steeringDir);

        float abs_R_inner = abs(R_inner), abs_R_outer = abs(R_outer);

        float obsDist = obs.distance(origo),
            innerDist = abs(obsDist - abs_R_inner),
            outerDist = abs(obsDist - abs_R_outer);

        // checks if car hits obstacle - if yes, minimum distance is negative
        td.dist = (Common::isBtw(obsDist, abs_R_inner, abs_R_outer) ? -1 : 1) * min(innerDist, outerDist);
        td.dir = static_cast<Common::RotationDir>((innerDist < outerDist ? 1 : -1) * static_cast<int>(pCar->steeringDir));

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
            td.remainingTime = R_rearFar * hitAngle / pCar->speed;
        }
    }

    return td;
}
