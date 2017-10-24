#ifndef DRIVE_FACTOR_HPP
#define DRIVE_FACTOR_HPP

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

#endif // DRIVE_FACTOR_HPP
