#include "Trajectory.hpp"

void Trajectory::updateRadiuses() {

	if (steeringAngle == 0)
		R_frontMiddle = R_rearMiddle = R_middle = R_outer = R_inner = 0;
	else {
		float sin_Angle = sin(steeringAngle), cos_Angle = cos(steeringAngle), tan_Angle = sin_Angle / cos_Angle;

		R_frontMiddle = CAR_PIVOT_DIST_FRONT_REAR / sin_Angle;
		R_rearMiddle = CAR_PIVOT_DIST_FRONT_REAR / tan_Angle;
		R_middle = Common::pythagoreanHypotenuse(R_rearMiddle, CAR_PIVOT_DIST_MID)
			* static_cast<int>(steeringDir);

		R_outer = Common::pythagoreanHypotenuse(R_rearMiddle + CAR_PIVOT_LENGTH_FRONT * static_cast<int>(steeringDir),
			CAR_PIVOT_DIST_FRONT_REAR + CAR_PIVOT_FRONT_DISTANCE)
			* static_cast<int>(steeringDir);

		R_inner = R_rearMiddle - CAR_PIVOT_LENGTH_REAR * static_cast<int>(steeringDir);

		R_frontNear = Common::pythagoreanHypotenuse(R_inner, CAR_PIVOT_DIST_FRONT_REAR) * static_cast<int>(steeringDir);
		R_rearFar = R_rearMiddle + CAR_PIVOT_LENGTH_REAR * static_cast<int>(steeringDir);
	}
}

void Trajectory::updateValues(int _steeringAngle, float _speed) {
	steeringAngle = _steeringAngle;
	steeringDir = steeringAngle > 0 ? Common::SteeringDir::LEFT : Common::SteeringDir::RIGHT;
	speed = _speed;

	updateRadiuses();
}

Trajectory::TrackDistance Trajectory::trackDistanceFromPoint(Point<float> relativePos, bool forceCalcRemainingTime) const {
	// origo	center of the trajectory circle of the rear pivot's center
	// obs		position of the obstacle relative to the rear pivot's center
	// inner	inner position of car when it is nearest to the object
	// outer	outer position of car when it is nearest to the object
	Point<float> origo(0.0, 0.0), obs, inner, outer;

	obs.X = relativePos.X + R_rearMiddle;
	obs.Y = relativePos.Y + CAR_PIVOT_DIST_MID;

	float obsAngle = origo.getSteeringAngle(obs, steeringDir);

	//float cos_ObsAngle = cos(obsAngle), sin_ObsAngle = sin(obsAngle);

	//inner.X = R_inner / cos_ObsAngle, inner.Y = R_inner / sin_ObsAngle,
	//outer.X = R_outer / cos_ObsAngle, outer.Y = R_outer / sin_ObsAngle;

	//float innerDist = inner.distanceFrom(obs),
	//outerDist = outer.distanceFrom(obs);

	float obsDist = obs.distanceFrom(origo),
		innerDist = abs(obsDist - abs(R_inner)),
		outerDist = abs(obsDist - abs(R_outer));

	TrackDistance td;
	// checks if car hits obstacle - if yes, minimum distance is negative
	td.dist = (Common::isBetween(obsDist, abs(R_inner), abs(R_outer)) ? -1 : 1) * min(innerDist, outerDist);
	td.dir = static_cast<Common::SteeringDir>((innerDist < outerDist ? 1 : -1) * static_cast<int>(steeringDir));

	// Calculates remaining time
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

	return td;
}

inline bool Trajectory::TrackDistance::isCritical() {
	return dist <= MIN_OBSTACLE_DISTANCE;
}
