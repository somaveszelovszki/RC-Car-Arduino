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

void Environment::SectionPointCalculator::setSection(const Point2f *pStartPoint, const Point2f *pEndPoint) {
    startPoint = *pStartPoint;
    Vec2f wholeDiff = *pEndPoint - startPoint;
    float wholeDiffLength = wholeDiff.length();

    pointsCount = min(wholeDiffLength / ENV_SECTION_POINTS_DIST + 1, ENV_SECTION_POINTS_MAX_NUM);
    diff = wholeDiff / pointsCount;
    currentPointIdx = 0;
}

Environment::Environment() : carPos(Point2f::ORIGO), carFwdAngle_Rad(M_PI_2), carFwdAngle_Cos(0.0f), carFwdAngle_Sin(1.0f) {
    for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x)
        for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y)
            envGrid.set(x, y, static_cast<uint2_t>(0));
}

Point2i rc_car::Environment::relPointToGridPoint(const Point2f& relPoint) const {
    Point2f absPos = carPos + Point2f(
        relPoint.X * carFwdAngle_Cos - relPoint.Y * carFwdAngle_Sin,
        relPoint.X * carFwdAngle_Sin + relPoint.Y * carFwdAngle_Cos);

    return Point2i(COORD_TO_GRID_POS(absPos.X), COORD_TO_GRID_POS(absPos.Y));
}

Point2i rc_car::Environment::getCarGridPos() const {
    return Point2i(COORD_TO_GRID_POS(carPos.X), COORD_TO_GRID_POS(carPos.Y));
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2i gridPos = relPointToGridPoint(relPoint);

    return gridPos.X < ENV_ABS_AXIS_POINTS_NUM
        && gridPos.Y < ENV_ABS_AXIS_POINTS_NUM
        && envGrid.get(gridPos.X, gridPos.Y);
}

void rc_car::Environment::updateGrid(const Point2f *sensedPoints[ULTRA_NUM_SENSORS]) {
    Point2f bottomLeft, topRight;
    Point2f::bbox(sensedPoints, ULTRA_NUM_SENSORS, &bottomLeft, &topRight);

    Point2i gridBottomLeft = relPointToGridPoint(bottomLeft),
        gridTopRight = relPointToGridPoint(topRight),
        carGridPos = getCarGridPos();

    for (int x = gridBottomLeft.X; x <= gridTopRight.X; ++x) {
        for (int y = gridBottomLeft.Y; y <= gridTopRight.Y; ++y) {
            Point2i current(x, y), p1 = relPointToGridPoint(*sensedPoints[ULTRA_NUM_SENSORS - 1]);

            // check if current point is inside polygon determined by the sensed points
            // iterates through all sensed points and checks if current point is in the triangle determined by the 2 adjacent sensed points and the car position
            bool isInside = false;
            for (int i = 0; !isInside && i < ULTRA_NUM_SENSORS; ++i) {
                Point2i p2 = relPointToGridPoint(*sensedPoints[i]);
                isInside = current.isInside(carGridPos, p1, p2);
                p1 = p2;
            }

            if (isInside)
                envGrid.decrement(x, y);        // not obstacle
            else
                envGrid.increment(x, y);        // obstacle
        }
    }

    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
        Common::UltrasonicPos pos1 = static_cast<Common::UltrasonicPos>(i), pos2 = Common::nextUltrasonicPos(pos1);
        // TODO calculate triangle (car middle and two adjacent sensed points)




    }

}
