#include "Environment.hpp"

using namespace rc_car;

/** @brief Converts relative coordinate to grid position.
Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define COORD_TO_GRID_POS(coord) static_cast<int>(coord > 0 ? (coord + (ENV_ABS_POINTS_DIST / 2)) / ENV_ABS_POINTS_DIST : (coord - (ENV_ABS_POINTS_DIST / 2)) / ENV_ABS_POINTS_DIST)

/** @brief Converts grid position to relative coordinate.
Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define GRID_POS_TO_COORD(pos) ((pos - ENV_ABS_AXIS_POINTS_NUM / 2) * ENV_ABS_POINTS_DIST)

void Environment::SectionPointCalculator::setSection(const Point2f& _startPoint, const Point2f& _endPoint) {
	startPoint = _startPoint;
	Vec2f wholeDiff = _endPoint - startPoint;
	float wholeDiffLength = wholeDiff.length();

	pointsCount = min(wholeDiffLength / ENV_SECTION_POINTS_DIST + 1, ENV_SECTION_POINTS_MAX_NUM);
	diff = wholeDiff / pointsCount;
	currentPointIdx = 0;
}

Environment::Environment() : carPos(Point2f::ORIGO), carFwdAngle_Rad(M_PI_2), carFwdAngle_Cos(0.0f), carFwdAngle_Sin(1.0f) {
	for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x)
		for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y)
			envGrid.set(x, y, false);
}

Point2i rc_car::Environment::relPointToGridPoint(const Point2f & relPoint) const {
    Point2f absPos = carPos + Point2f(
        relPoint.X * carFwdAngle_Cos - relPoint.Y * carFwdAngle_Sin,
        relPoint.X * carFwdAngle_Sin + relPoint.Y * carFwdAngle_Cos);

    return Point2i(COORD_TO_GRID_POS(absPos.X), COORD_TO_GRID_POS(absPos.Y));
}

void Environment::setRelativePointObstacle(const Point2f& relPoint) {
	// TODO handle grid overflow

    Point2i gridPos = relPointToGridPoint(relPoint);

	if (static_cast<unsigned int>(gridPos.X) < ENV_ABS_AXIS_POINTS_NUM && static_cast<unsigned int>(gridPos.Y) < ENV_ABS_AXIS_POINTS_NUM)
		envGrid.set(gridPos.X, gridPos.Y, true);
#if __DEBUG && false
	else
		DEBUG_println("Invalid grid positions: " + String(x) + ", " + String(y) + " - " + point.toString());
#endif
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2i gridPos = relPointToGridPoint(relPoint);

    return static_cast<unsigned int>(gridPos.X) < ENV_ABS_AXIS_POINTS_NUM
        && static_cast<unsigned int>(gridPos.Y) < ENV_ABS_AXIS_POINTS_NUM
        && envGrid.get(gridPos.X, gridPos.Y);
}
