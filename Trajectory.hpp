#ifndef RC_CAR__TRAJECTORY__HPP
#define RC_CAR__TRAJECTORY__HPP

#include "Point2.hpp"

namespace rc_car {
	/** @brief Calculates car trajectory from steering angle, and calculates object distance from this trajectory.
	*/
	class Trajectory {
	public:

		class TrackDistance {
		public:
			float dist;	// distance from side of car to obstacle - if dist < 0 then car hits obstacle
			Common::SteeringDir dir;
			float remainingTime;	// time before reaching the point of the trajectory when car is nearest to (or when it hits) the obstacle
			bool isCritical() const {
				return dist <= MIN_OBSTACLE_DIST;
			}
		};

	private:

		float speed;
		float steeringAngle;
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

		void updateValues(float _speed, float _steeringAngle);

		/** @brief Calculates distance between given point and car during its trajectory.
		@param relativePos coordinates of the point - relative to car middle.
		@param forceCalcRemainingTime Indicates if remaining time should be calculated even if car will not hit obstacle
		*/
		TrackDistance trackDistanceFromPoint(const Point2f& relativePos, bool forceCalcRemainingTime = false) const;
	};
}

#endif // RC_CAR__TRAJECTORY__HPP
