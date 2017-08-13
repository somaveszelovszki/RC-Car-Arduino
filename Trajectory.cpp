#include "Trajectory.hpp"

void Trajectory::updateRadiuses() {

	if (steeringAngleRad == 0)
		R_frontMiddle = R_rearMiddle = R_middle = R_outer = R_inner = 0;
	else {
		double sin_Angle = sin(steeringAngleRad), cos_Angle = cos(steeringAngleRad), tan_Angle = sin_Angle / cos_Angle,
			frontCornerToPivotAngle = atan(CAR_PIVOT_FRONT_DISTANCE / CAR_PIVOT_LENGTH_FRONT);

		// frontCornerToPivotAngle can be set as a compile-time constant
		// cos(frontCornerToPivotAngle) as well

		R_frontMiddle = CAR_PIVOT_DISTANCE_FRONT_REAR / sin_Angle;
		R_rearMiddle = CAR_PIVOT_DISTANCE_FRONT_REAR / tan_Angle;
		R_middle = Common::pythagoreanHypotenuse(R_rearMiddle, CAR_PIVOT_DISTANCE_MIDDLE)
			* static_cast<int>(steeringDir);

		R_outer = R_frontMiddle +
			Common::pythagoreanHypotenuse(CAR_PIVOT_LENGTH_FRONT, CAR_PIVOT_FRONT_DISTANCE)
			* cos(steeringAngleRad - frontCornerToPivotAngle)
			/ cos(frontCornerToPivotAngle)
			* static_cast<int>(steeringDir);

		R_inner = R_rearMiddle - CAR_PIVOT_LENGTH_REAR * static_cast<int>(steeringDir);

		R_frontNear = Common::pythagoreanHypotenuse(R_inner, CAR_PIVOT_DISTANCE_FRONT_REAR) * static_cast<int>(steeringDir);
		R_rearFar = R_middle + CAR_PIVOT_LENGTH_REAR * static_cast<int>(steeringDir);
	}
}

void Trajectory::updateValues(int steeringAngle, double speed) {
	steeringAngleRad = Common::degreeToRadian(static_cast<double>(steeringAngle));
	steeringDir = steeringAngleRad > 0 ? LEFT : RIGHT;
	this->speed = speed;

	updateRadiuses();
}

Trajectory::TrackDistance Trajectory::trackDistanceFromPoint(Point<double> relativePos, bool forceCalcRemainingTime) {
	// origo	center of the trajectory circle of the rear pivot's center
	// obs		position of the obstacle relative to the rear pivot's center
	// inner	inner position of car when it is nearest to the object
	// outer	outer position of car when it is nearest to the object
	Point<double> origo(0.0, 0.0), obs, inner, outer;

	obs.X = relativePos.X + R_inner;
	obs.Y = relativePos.Y - CAR_PIVOT_DISTANCE_MIDDLE;

	double obsAngle = origo.getAngle(obs);

	double cos_ObsAngle = cos(obsAngle), sin_ObsAngle = sin(obsAngle);

	inner.X = R_inner / cos_ObsAngle, inner.Y = R_inner / sin_ObsAngle,
		outer.X = R_outer / cos_ObsAngle, outer.Y = R_outer / sin_ObsAngle;

	double innerDist = inner.distanceFrom(obs),
		outerDist = outer.distanceFrom(obs);

	TrackDistance td;
	// checks if car hits obstacle - if yes, minimum distance is negative
	td.dist = (Common::isBetween(obs.X, inner.X, outer.X) ? -1 : 1) * min(innerDist, outerDist);
	td.dir = static_cast<STEERING_DIRECTION>((innerDist < outerDist ? 1 : -1) * static_cast<int>(steeringDir));

	// Calculates remaining time
	if (td.isCritical() || forceCalcRemainingTime) {

		// delta angle on the trajectory
		// -> the given part of the car that hits the obstacle will reach it before the rear pivot does
		// this angle specifies this difference
		double dAngle = 0.0;

		if (td.isCritical()) {
			if (innerDist < outerDist) {
				if (innerDist < R_frontNear - R_inner)
					dAngle = atan(CAR_PIVOT_DISTANCE_FRONT_REAR / R_inner) * (innerDist / abs(R_frontNear - R_inner));
				else
					dAngle = atan(CAR_PIVOT_DISTANCE_FRONT_REAR / R_inner);
			} else
				dAngle = atan(CAR_PIVOT_DISTANCE_FRONT_REAR / R_inner);
		}

		double hitAngle = obsAngle + dAngle;
		td.remainingTime = R_rearFar * hitAngle / speed;
	}

	return td;
}

inline bool Trajectory::TrackDistance::isCritical() {
	return dist <= MIN_OBSTACLE_DISTANCE;
}
