#ifndef RC_CAR__DRIVE_FACTOR__HPP
#define RC_CAR__DRIVE_FACTOR__HPP

namespace rc_car {

	/*
	Drive factors:
		environment
		speed
		steering angle
		destination
		previously reached locations
	*/

	class DriveFactor {
	public:
		float weight;
	};
}

#endif // RC_CAR__DRIVE_FACTOR__HPP
