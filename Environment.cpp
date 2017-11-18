#include "Environment.hpp"

using namespace rc_car;

#define COORD_TO_GRID_POS(pos) ((pos) >= 0 ? static_cast<int>(pos) / ENV_ABS_POINTS_DIST : ((pos) - (ENV_ABS_POINTS_DIST - 1)) / ENV_ABS_POINTS_DIST) + ENV_ABS_AXIS_POINTS_NUM / 2

void Environment::SectionPointCalculator::setSection(const Point2f& _startPoint, const Point2f& _endPoint) {
	startPoint = _startPoint;
	Vec2f wholeDiff = _endPoint - startPoint;
	float wholeDiffLength = wholeDiff.length();

	pointsCount = min(wholeDiffLength / ENV_SECTION_POINTS_DIST + 1, ENV_SECTION_POINTS_MAX_NUM);
	diff = wholeDiff / (pointsCount + 1);
	currentPointIdx = 0;
}

Environment::Environment() : carPos(Point2f::ORIGO), carFwdDir_Rad(M_PI_2), carFwdDir_Cos(0.0f), carFwdDir_Sin(1.0f) {
	for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x)
		for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y)
			envGrid.set(x, y, false);
}

void Environment::setRelativePointObstacle(const Point2f& relPoint) {
	// TODO handle grid overflow

	int x = COORD_TO_GRID_POS(relPoint.X), y = COORD_TO_GRID_POS(relPoint.Y);
	if (static_cast<unsigned int>(x) < ENV_ABS_AXIS_POINTS_NUM && static_cast<unsigned int>(y) < ENV_ABS_AXIS_POINTS_NUM)
		envGrid.set(x, y, true);
#if __DEBUG && false
	else
		DEBUG_println("Invalid grid positions: " + String(x) + ", " + String(y) + " - " + point.toString());
#endif
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
	int x = COORD_TO_GRID_POS(relPoint.X), y = COORD_TO_GRID_POS(relPoint.Y);
	bool isObstacle;
	if (static_cast<unsigned int>(x) < ENV_ABS_AXIS_POINTS_NUM && static_cast<unsigned int>(y) < ENV_ABS_AXIS_POINTS_NUM)
		isObstacle = envGrid.get(x, y);
#if __DEBUG && false
	else {
		DEBUG_println("Invalid grid positions: " + String(x) + ", " + String(y) + " - " + point.toString());
		isObstacle = false;
	}
#endif

	return isObstacle;
}
