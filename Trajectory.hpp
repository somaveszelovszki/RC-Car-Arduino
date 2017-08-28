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
		double dist;	// distance from side of car to obstacle - if dist < 0 then car hits obstacle
		Common::SteeringDir dir;
		double remainingTime;	// time before reaching the point of the trajectory when car is nearest to (or when it hits) the obstacle
		bool isCritical();
	};

private:

	double speed;
	double steeringAngleRad;
	Common::SteeringDir steeringDir;

	double R_frontMiddle;
	double R_rearMiddle;
	double R_middle;

	double R_inner;
	double R_outer;
	double R_frontNear;
	double R_rearFar;

	void updateRadiuses();

public:

	void updateValues(int steeringAngle, double speed);

	/*
	Calculates distance between given point and car during its trajectory.
	@param relativePos coordinates of the point - relative to car middle.
	@param forceCalcRemainingTime Indicates if remaining time should be calculated even if car will not hit obstacle
	*/
	TrackDistance trackDistanceFromPoint(Point<double> relativePos, bool forceCalcRemainingTime = false);
};

#endif		// TRAJECTORY_HPP