#include "Environment.hpp"

using namespace rc_car;

//void Environment::calculateSectionPoints(Point<float> measuredPoints[ULTRA_NUM_SENSORS], Common::UltrasonicPos startPos, int sectionIdx) {
//	Point<float> start = measuredPoints[startPos],
//		end = measuredPoints[Common::nextUltrasonicPos(startPos)],
//		wholeDiff = end - start;
//
//	float wholeDiffLength = wholeDiff.length();
//	int sectionPointsCount = min(wholeDiffLength / ENV_SECTION_POINTS_DIST, ENV_SECTION_POINTS_MAX_NUM);
//
//	Point<float> diff = wholeDiff / (sectionPointsCount + 1);
//
//	for (int i = 0; i < sectionPointsCount; ++i)
//		envPoints[sectionIdx][i] = start + i * diff;
//}

void Environment::setSection(const Point<float>& _startPoint, const Point<float>& _endPoint) {
	startPoint = _startPoint;
	Point<float> wholeDiff = _endPoint - startPoint;
	float wholeDiffLength = wholeDiff.length();

	sectionPointsCount = min(wholeDiffLength / ENV_SECTION_POINTS_DIST, ENV_SECTION_POINTS_MAX_NUM);
	diff = wholeDiff / (sectionPointsCount + 1);
	currentSectionPointIdx = 0;
}

Point<float> Environment::calculateNextSectionPoint() {
	return startPoint + diff * currentSectionPointIdx++;
}

//void Environment::calculate(Point<float> measuredPoints[ULTRA_NUM_SENSORS], Common::UltrasonicPos startPos) {
//	for (int i = 0; i < ENV_SECTIONS_NUM; ++i)
//		calculateSectionPoints(measuredPoints, static_cast<Common::UltrasonicPos>(static_cast<int>(startPos) + i), i);
//}