#include "Environment.hpp"

using namespace rc_car;

void Environment::calculateSectionPoints(Common::UltrasonicPos pos, int *pDestStartIndex) {
	//Point<float> start = measuredPoints[pos],
	//	end = measuredPoints[Common::nextUltrasonicPos(pos)];

	//float diffAngle = abs(Point<float>::ORIGO.getAngle(start, Common::SteeringDir::LEFT)
	//	- Point<float>::ORIGO.getAngle(end, Common::SteeringDir::LEFT));

	//// rounds down angle ratio -> gets number of middle points
	//int middlePointsNum = static_cast<int>(diffAngle / ENV_POINTS_DELTA_ANGLE);

	//Point<float> diff = (end - start) / (middlePointsNum + 1);
	//for (int i = 0; i < middlePointsNum; ++i)
	//	estimatedPoints[(*pDestStartIndex)++] = start + i * diff;
}

void Environment::calculate() {
	int currentIndex = 0;
	for (int i = 0; i < ULTRA_NUM_SENSORS; ++i)
		calculateSectionPoints(static_cast<Common::UltrasonicPos>(i), &currentIndex);
}