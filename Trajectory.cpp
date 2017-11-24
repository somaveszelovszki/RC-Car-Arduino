#include "Trajectory.hpp"

using namespace rc_car;

void Trajectory::updateRadiuses() {

	if (Common::areEqual(steeringAngle, 0.0f))
		R_frontMiddle = R_rearMiddle = R_middle = R_outer = R_inner = 0.0f;
	else {
		float sin_Angle = sin(steeringAngle), cos_Angle = cos(steeringAngle), tan_Angle = sin_Angle / cos_Angle;

		R_frontMiddle = CAR_PIVOT_DIST_FRONT_REAR / sin_Angle;
		R_rearMiddle = CAR_PIVOT_DIST_FRONT_REAR / tan_Angle;
		R_middle = Common::pythagoreanHypotenuse(R_rearMiddle, CAR_PIVOT_DIST_MID)
			* static_cast<int>(steeringDir);

		R_outer = Common::pythagoreanHypotenuse(R_rearMiddle + CAR_PIVOT_LENGTH * static_cast<int>(steeringDir),
			CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_FRONT_DIST)
			* static_cast<int>(steeringDir);

		R_inner = R_rearMiddle - CAR_PIVOT_LENGTH * static_cast<int>(steeringDir);

		R_frontNear = Common::pythagoreanHypotenuse(R_inner, CAR_PIVOT_DIST_FRONT_REAR) * static_cast<int>(steeringDir);
		R_rearFar = R_rearMiddle + CAR_PIVOT_LENGTH * static_cast<int>(steeringDir);
	}
}

void Trajectory::updateValues(float _speed, float _steeringAngle) {
	steeringAngle = _steeringAngle;
	steeringDir = steeringAngle >= 0.0f ? Common::SteeringDir::LEFT : Common::SteeringDir::RIGHT;
	speed = _speed;

	updateRadiuses();
}

Trajectory::TrackDistance Trajectory::trackDistanceFromPoint(const Point2f& relativePos, bool forceCalcRemainingTime) const {

	TrackDistance td;

	if (R_middle) {
		// origo	center of the trajectory circle of the rear pivot's center
		// obs		position of the obstacle relative to the rear pivot's center
		Point2f origo(-R_rearMiddle, 0.0), obs;

		obs.X = relativePos.X + R_rearMiddle;
		obs.Y = relativePos.Y + CAR_PIVOT_DIST_MID;

		float obsAngle = origo.getAngle(obs, steeringDir);

		float cos_ObsAngle = cos(obsAngle), sin_ObsAngle = sin(obsAngle);

		float obsDist = R_inner ? obs.distanceFrom(origo) : abs(obs.X),
			innerDist = abs(obsDist - abs(R_inner)),
			outerDist = abs(obsDist - abs(R_outer));

		// checks if car hits obstacle - if yes, minimum distance is negative
		td.dist = (Common::isBetween(obsDist, abs(R_inner), abs(R_outer)) ? -1 : 1) * min(innerDist, outerDist);
		td.dir = static_cast<Common::SteeringDir>((innerDist < outerDist ? 1 : -1) * static_cast<int>(steeringDir));

		// calculates remaining time
		if (td.isCritical() || forceCalcRemainingTime) {

			// delta angle on the trajectory
			// -> the given part of the car that hits the obstacle will reach it before the rear pivot does
			// this angle specifies this difference
			float dAngle = 0.0;

			if (td.isCritical()) {
				if (innerDist < outerDist) {
					if (innerDist < abs(R_frontNear - R_inner))
						dAngle = atan(CAR_PIVOT_DIST_FRONT_REAR / R_inner)
						* (innerDist / abs(R_frontNear - R_inner));
					else
						dAngle = atan(CAR_PIVOT_DIST_FRONT_REAR / R_inner);
				} else
					dAngle = atan(CAR_PIVOT_DIST_FRONT_REAR / R_inner);
			}


			float hitAngle = obsAngle - dAngle;

			td.remainingTime = R_rearFar * hitAngle / speed;
		}
	} else {
		float innerDist = abs(-CAR_PIVOT_LENGTH - relativePos.X),
			outerDist = abs(CAR_PIVOT_LENGTH - relativePos.X);

		// checks if car hits obstacle - if yes, minimum distance is negative
		td.dist = (Common::isBetween(relativePos.X, -CAR_PIVOT_LENGTH, CAR_PIVOT_LENGTH) ? -1 : 1) * min(innerDist, outerDist);
		td.dir = Common::SteeringDir::LEFT;

		// calculates remaining time
		if (td.isCritical() || forceCalcRemainingTime)
			td.remainingTime = (relativePos.Y - (CAR_HEIGHT / 2) * (relativePos.Y > 0 ? 1 : -1)) / speed;
	}

	return td;
}
