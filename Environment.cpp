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

void Environment::SectionPointCalculator::setSection(const Point2f *pStartPoint, const Point2f *pEndPoint, float fixDiff) {
    startPoint = *pStartPoint;
    Vec2f wholeDiff = *pEndPoint - startPoint;
    float wholeDiffLength = wholeDiff.length();

    pointsCount = fixDiff ? wholeDiffLength / fixDiff : min(wholeDiffLength / ENV_SECTION_POINTS_DIST + 1, ENV_SECTION_POINTS_MAX_NUM);
    diff = wholeDiff / pointsCount;
    currentPointIdx = 0;
}

Environment::Environment(const Point2f *_sensedPoints) :
    carPos(0.0f, 0.0f),
    carFwdAngle_Rad(M_PI_2),
    carFwdAngle_Cos(0.0f),
    carFwdAngle_Sin(1.0f),
    sensedPoints(_sensedPoints) {

    for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x) {
        for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y) {
            grid.set(x, y, static_cast<uint2_t>(0));
        }
    }

    gridWriter.setGrid(&grid);
}

Point2ui8 Environment::relPointToGridPoint(const Point2f& relPoint) const {
    Point2f absPos = carPos + Point2f(
        relPoint.X * carFwdAngle_Cos - relPoint.Y * carFwdAngle_Sin,
        relPoint.X * carFwdAngle_Sin + relPoint.Y * carFwdAngle_Cos);

    return Point2ui8(COORD_TO_GRID_POS(absPos.X), COORD_TO_GRID_POS(absPos.Y));
}

Point2ui8 Environment::getCarGridPos() const {
    return Point2ui8(COORD_TO_GRID_POS(carPos.X), COORD_TO_GRID_POS(carPos.Y));
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2ui8 gridPos = relPointToGridPoint(relPoint);

    return gridPos.X < ENV_ABS_AXIS_POINTS_NUM
        && gridPos.Y < ENV_ABS_AXIS_POINTS_NUM
        && grid.get(gridPos.X, gridPos.Y);
}

void Environment::updateGrid() {
    //DEBUG_println("updateGrid");
    const Point2f *pSectionStart, *pSectionEnd;
    Common::UltrasonicPos sectionStartPos = Common::UltrasonicPos::RIGHT_FRONT;

    pSectionStart = &sensedPoints[sectionStartPos];

    // updates all sections in the grid (obstacle probability of the points between the sensed points are incremented, inner points are decremented)
    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {

        pSectionEnd = &sensedPoints[sectionStartPos = Common::nextUltrasonicPos(sectionStartPos)];
        sectionPointCalculator.setSection(pSectionStart, pSectionEnd, ENV_ABS_POINTS_DIST);

        Point2ui8 carGridPos = relPointToGridPoint(carPos),
            prevGridPoint = carGridPos;     // section points cannot be where the car is, so the car's position is a good initial value

        int currentGridPointIdx = 0;

        // iterates through section points, adds them to the section grid points array,
        // and increments obstacle probabilities (section points are sensed as obstacles)
        while (sectionPointCalculator.nextExists() && currentGridPointIdx < ENV_ABS_SECTION_POINTS_MAX_NUM) {

            Point2ui8 gridPoint = relPointToGridPoint(sectionPointCalculator.next());
            if (gridPoint != prevGridPoint) {

                sectionGridPoints[currentGridPointIdx++] = gridPoint;

                grid.increment(gridPoint.X, gridPoint.Y);
                prevGridPoint = gridPoint;
            }
        }

        int sectionGridPointsNum = currentGridPointIdx;

        Point2ui8 bl, tr,     // bottom left, top right corners of the triangle bounding box
            startGridPos = relPointToGridPoint(*pSectionStart),
            endGridPos = relPointToGridPoint(*pSectionEnd);

        Point2ui8 triangle[3] = {
            carGridPos,
            startGridPos,
            endGridPos
        };

        // checks points defined by the bounding box of the car position and the start and end points if the are inside the triangle defined by these points
        Point2ui8::bbox(triangle, 3, &bl, &tr);
        for (int x = bl.X; x <= tr.X; ++x) {
            for (int y = bl.Y; y <= tr.Y; ++y) {
                Point2ui8 current(x, y);
                // checks if current point is inside the triangle determined by the car position and start and end points
                // if yes (and it is not a section point, then decrements its obstacle probability, because it has not been sensed as an obstacle)
                if (current.isInside(carGridPos, startGridPos, endGridPos) && !Common::contains(current, sectionGridPoints, sectionGridPointsNum))
                    grid.decrement(current.X, current.Y);
            }
        }

        pSectionStart = pSectionEnd;
    }
}

Point2ui8 Environment::nextToStream(ByteArray<COMM_MSG_DATA_LENGTH>& result) {
    Point2ui8 firstByte;
    gridWriter.next(result, COMM_MSG_DATA_LENGTH, &firstByte.X, &firstByte.Y);
    return firstByte;
}
