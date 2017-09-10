#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "Point.hpp"

/*
Calculates car trajectory from steering angle
*/
class Trajectory {
public:

	class TrackDistance {
	public:
		float dist;	// distance from side of car to obstacle - if dist < 0 then car hits obstacle
		Common::SteeringDir dir;
		float remainingTime;	// time before reaching the point of the trajectory when car is nearest to (or when it hits) the obstacle
		bool isCritical();
	};

private:

	float speed;
	float steeringAngleRad;
	Common::SteeringDir steeringDir;

	float R_frontMiddle;
	float R_rearMiddle;
	float R_middle;

	float R_inner;
	float R_outer;
	float R_frontNear;
	float R_rearFar;

	void updateRadiuses();

public:

	void updateValues(int steeringAngle, float speed);

	/*
	Calculates distance between given point and car during its trajectory.
	@param relativePos coordinates of the point - relative to car middle.
	@param forceCalcRemainingTime Indicates if remaining time should be calculated even if car will not hit obstacle
	*/
	TrackDistance trackDistanceFromPoint(Point<float> relativePos, bool forceCalcRemainingTime = false) const;
};

#endif		// TRAJECTORY_HPP