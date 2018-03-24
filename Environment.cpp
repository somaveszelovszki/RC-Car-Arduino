#include "Environment.hpp"

using namespace rc_car;

/** @brief Converts relative X coordinate to grid position. Idxinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define COORD_TO_GRID_POS_X(coord) static_cast<int32_t>(ENV_ABS_AXIS_POINTS_NUM / 2 + coord / ENV_ABS_POINTS_DIST)

/** @brief Converts relative Y coordinate to grid position. Idxinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define COORD_TO_GRID_POS_Y(coord) static_cast<int32_t>(ENV_ABS_AXIS_POINTS_NUM / 2 - coord / ENV_ABS_POINTS_DIST)

/** @brief Converts grid X position to relative coordinate. Idxinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define GRID_POS_TO_COORD_X(pos) ((pos - ENV_ABS_AXIS_POINTS_NUM / 2) * ENV_ABS_POINTS_DIST)

/** @brief Converts grid Y position to relative coordinate. Idxinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define GRID_POS_TO_COORD_Y(pos) ((ENV_ABS_AXIS_POINTS_NUM / 2 - pos) * ENV_ABS_POINTS_DIST)

void Environment::SectionPointCalculator::setSection(const Point2f *pStartPoint, const Point2f *pEndPoint, float fixDiff) {
    startPoint = *pStartPoint;
    Vec2f wholeDiff = *pEndPoint - startPoint;
    float wholeDiffLength = wholeDiff.length();

    pointsCount = fixDiff ? wholeDiffLength / fixDiff : min(wholeDiffLength / ENV_SECTION_POINTS_DIST + 1, ENV_SECTION_POINTS_MAX_NUM);
    diff = wholeDiff / pointsCount;
    currentPointIdx = 0;
}

Environment::Environment(const CarProps *_pCar, const Point2f *_sensedPoints) : pCar(_pCar), sensedPoints(_sensedPoints) {

    for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x) {
        for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y) {
            grid.set(x, y, static_cast<uint2_t>(0));
        }
    }

    gridWriter.setGrid(&grid);
}

Point2ui8 Environment::relPointToGridPoint(const Point2f& relPoint) const {

    // car pos + relative pos rotated with (fwdAngle - 90 degrees)
    Point2f absPos = pCar->pos + Point2f(
        relPoint.X * pCar->fwdAngle_Sin + relPoint.Y * pCar->fwdAngle_Cos,
        - relPoint.X * pCar->fwdAngle_Cos + relPoint.Y * pCar->fwdAngle_Sin);

    return Point2ui8(toRealIdx(COORD_TO_GRID_POS_X(absPos.X)), toRealIdx(COORD_TO_GRID_POS_Y(absPos.Y)));
}

uint8_t rc_car::Environment::toRealIdx(int32_t idx) {
    return static_cast<uint8_t>((idx < 0 || idx >= ENV_ABS_AXIS_POINTS_NUM) ? ENV_ABS_AXIS_POINTS_NUM : idx);
}

Point2ui8 Environment::getCarGridPoint() const {
    return Point2ui8(toRealIdx(COORD_TO_GRID_POS_X(pCar->pos.X)), toRealIdx(COORD_TO_GRID_POS_Y(pCar->pos.Y)));
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2ui8 gridPos = relPointToGridPoint(relPoint);

    return gridPos.X < ENV_ABS_AXIS_POINTS_NUM
        && gridPos.Y < ENV_ABS_AXIS_POINTS_NUM
        && grid.get(gridPos.X, gridPos.Y) >= (1 << (ENV_ABS_POINTS_BIT_DEPTH - 1));
}

void Environment::updateGrid() {
    // updates all sections in the grid (obstacle probability of the points between the sensed points are incremented, inner points are decremented)
    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
        Point2ui8 gridPoint = relPointToGridPoint(sensedPoints[i]);
        grid.increment(gridPoint.X, gridPoint.Y);
    }
}

Point2ui8 Environment::nextToStream(ByteArray<COMM_MSG_DATA_LENGTH>& result) {
    Point2ui8 firstByte;
    gridWriter.next(result, COMM_MSG_DATA_LENGTH, &firstByte.X, &firstByte.Y);
    return firstByte;
}
