#include "Environment.hpp"

using namespace rc_car;

/** @brief Converts relative X coordinate to grid position. Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define COORD_TO_GRID_POS_X(coord) static_cast<uint8_t>(ENV_ABS_AXIS_POINTS_NUM / 2 + coord / ENV_ABS_POINTS_DIST)

/** @brief Converts relative Y coordinate to grid position. Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define COORD_TO_GRID_POS_Y(coord) static_cast<uint8_t>(ENV_ABS_AXIS_POINTS_NUM / 2 - coord / ENV_ABS_POINTS_DIST)

/** @brief Converts grid X position to relative coordinate. Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
*/
#define GRID_POS_TO_COORD_X(pos) ((pos - ENV_ABS_AXIS_POINTS_NUM / 2) * ENV_ABS_POINTS_DIST)

/** @brief Converts grid Y position to relative coordinate. Coordinate is relative to car, grid position is a position in the grid (0-indexed 2-dimensional array).
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
    /*Point2f absPos = pCar->pos + Point2f(
        relPoint.X * pCar->fwdAngle_Sin + relPoint.Y * pCar->fwdAngle_Cos,
        - relPoint.X * pCar->fwdAngle_Cos + relPoint.Y * pCar->fwdAngle_Sin);TODO*/

    Point2f absPos = relPoint;

    return Point2ui8(COORD_TO_GRID_POS_X(absPos.X), COORD_TO_GRID_POS_Y(absPos.Y));
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2ui8 gridPos = relPointToGridPoint(relPoint);

    return gridPos.X < ENV_ABS_AXIS_POINTS_NUM
        && gridPos.Y < ENV_ABS_AXIS_POINTS_NUM
        && grid.get(gridPos.X, gridPos.Y) >= 2;
}

void Environment::updateGrid() {

    // TODO remove these resetting lines
    for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x) {
        for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y) {
            grid.set(x, y, static_cast<uint2_t>(0));
        }
    }

    const Point2f *pSectionStart, *pSectionEnd;
    Common::UltrasonicPos sectionStartPos = Common::UltrasonicPos::RIGHT_FRONT;

    pSectionStart = &sensedPoints[sectionStartPos];

    // updates all sections in the grid (obstacle probability of the points between the sensed points are incremented, inner points are decremented)
    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {

        pSectionEnd = &sensedPoints[sectionStartPos = Common::nextUltrasonicPos(sectionStartPos)];
        sectionPointCalculator.setSection(pSectionStart, pSectionEnd, ENV_ABS_POINTS_DIST);

        //DEBUG_print("car: (");
        //DEBUG_print(pCar->pos.X);
        //DEBUG_print(", ");
        //DEBUG_print(pCar->pos.Y);
        //DEBUG_println(")");

        Point2ui8 carGridPos = relPointToGridPoint(pCar->pos), prevGridPoint = carGridPos;     // section points cannot be where the car is, so the car's position is a good initial value

        //DEBUG_print("car idx: (");
        //DEBUG_print(carGridPos.X);
        //DEBUG_print(", ");
        //DEBUG_print(carGridPos.Y);
        //DEBUG_println(")");

        int currentGridPointIdx = 0;

        //DEBUG_print("Section: (");
        //DEBUG_print(pSectionStart->X);
        //DEBUG_print(", ");
        //DEBUG_print(pSectionStart->Y);
        //DEBUG_print(") - (");
        //DEBUG_print(pSectionEnd->X);
        //DEBUG_print(", ");
        //DEBUG_print(pSectionEnd->Y);
        //DEBUG_println(")");


        // iterates through section points, adds them to the section grid points array,
        // and increments obstacle probabilities (section points are sensed as obstacles)
        while (sectionPointCalculator.nextExists() && currentGridPointIdx < ENV_ABS_SECTION_POINTS_MAX_NUM - 1) {

            Point2ui8 gridPoint = relPointToGridPoint(sectionPointCalculator.next());

            //DEBUG_print("grid point: (");
            //DEBUG_print(gridPoint.X);
            //DEBUG_print(", ");
            //DEBUG_print(gridPoint.Y);
            //DEBUG_println(")");

            if (gridPoint != prevGridPoint) {
                grid.increment(gridPoint.X, gridPoint.Y);
                sectionGridPoints[currentGridPointIdx++] = prevGridPoint = gridPoint;
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

        //DEBUG_print("start: (");
        //DEBUG_print(startGridPos.X);
        //DEBUG_print(", ");
        //DEBUG_print(startGridPos.Y);
        //DEBUG_println(")");
        //
        //DEBUG_print("end: (");
        //DEBUG_print(endGridPos.X);
        //DEBUG_print(", ");
        //DEBUG_print(endGridPos.Y);
        //DEBUG_println(")");

        // checks points defined by the bounding box of the car position and the start and end points if the are inside the triangle defined by these points
        Point2ui8::bbox(triangle, 3, &bl, &tr);

        //DEBUG_println(String("bbox: (") + bl.X + ", " + bl.Y + ") - (" + tr.X + ", " + tr.Y + ")");

        //DEBUG_print("\nbbox: (");
        //DEBUG_print(bl.X);
        //DEBUG_print(", ");
        //DEBUG_print(bl.Y);
        //DEBUG_print(") - (");
        //DEBUG_print(tr.X);
        //DEBUG_print(", ");
        //DEBUG_print(tr.Y);
        //DEBUG_println(")");


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
