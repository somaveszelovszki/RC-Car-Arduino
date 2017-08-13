#ifndef TRAJECTORY_HPP
#define TRAJECTORY_HPP

#include "Point.hpp"
#include "CarSpecs.hpp"

// minimum distance to keep from obstacles - in [cm]
// if trajectory is within this distance from an obstacle, will be handled as if a crash was going to happen
#define MIN_OBSTACLE_DISTANCE 2.0

/*
Calculates car trajectory from steering angle
*/
class Trajectory {
public:

	enum STEERING_DIRECTION {
		LEFT = 1,
		RIGHT = -1
	};

	class TrackDistance {
	public:
		double dist;	// distance from side of car to obstacle - if dist < 0 then car hits obstacle
		STEERING_DIRECTION dir;
		double remainingTime;	// time before reaching the point of the trajectory when car is nearest to (or when it hits) the obstacle
		bool isCritical();
	};

private:

	double speed;
	double steeringAngleRad;
	STEERING_DIRECTION steeringDir;

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

	/*
	Calculates current distance between given point and car.
	X and Y coordinates of the point are relative to car middle.
	TODO implement it if needed
	*/
	//TrackDistance currentDistanceFromPoint(Point<double> relativePos);
};

#endif		// TRAJECTORY_HPP