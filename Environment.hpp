#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Point.hpp"
#include "Vec.hpp"

namespace rc_car {
	/** @brief Calculates environment around car from the measured data of the ultrasonic sensors.
	*/
	class Environment {
	private:
		Point<float> currentPos;
		Point<float> absPoints[ENV_ABS_POINTS_NUM_X][ENV_ABS_POINTS_NUM_Y];

		void calculateSectionPoints(Common::UltrasonicPos pos, int *pDestStartIndex);

	public:
		Point<float> measuredPoints[ULTRA_NUM_SENSORS];
		Point<float> estimatedPoints[ENV_POINTS_NUM];

		void calculate();
	};
}

#endif // ENVIRONMENT_HPP
