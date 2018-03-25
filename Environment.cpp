#include "Environment.hpp"

using namespace rc_car;

void Environment::SectionPointCalculator::setSection(const Point2f *pStartPoint, const Point2f *pEndPoint, float targetDiff, uint8_t maxPointsNum) {
    startPoint = *pStartPoint;
    Vec2f wholeDiff = *pEndPoint - startPoint;

    pointsCount = wholeDiff.length() / targetDiff + 1;
    if (maxPointsNum && maxPointsNum < pointsCount)
        pointsCount = maxPointsNum;

    diff = wholeDiff / pointsCount;
    currentPointIdx = 0;
}

Environment::Environment(const CarProps *_pCar, const Point2f *_sensedPoints) : pCar(_pCar), sensedPoints(_sensedPoints) {

    for (int x = 0; x < ENV_GRID_AXIS_NUM_POINTS; ++x) {
        for (int y = 0; y < ENV_GRID_AXIS_NUM_POINTS; ++y) {
            grid.set(x, y, static_cast<uint2_t>(0));
        }
    }

    gridWriter.setGrid(&grid);
}

bool Environment::isRelativePointObstacle(const Point2f& relPoint) const {
    Point2ui8 gridPos = relPointToGridPoint<uint8_t>(relPoint);

    return gridPos.X < ENV_GRID_AXIS_NUM_POINTS
        && gridPos.Y < ENV_GRID_AXIS_NUM_POINTS
        && grid.get(gridPos.X, gridPos.Y) >= (1 << (ENV_GRID_POINT_BIT_DEPTH - 1));
}

void Environment::updateGrid() {
    // updates all sections in the grid:
    // obstacle probability of the points between the sensed points are incremented, inner points are decremented

    Point2f carGridPos = relPointToGridPoint<float>(pCar->pos);

    for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
        Point2f sensedGridPoint = relPointToGridPoint<float>(sensedPoints[i]);
        Point2ui8 prevGridPoint = static_cast<Point2ui8>(sensedGridPoint);

        grid.increment(sensedGridPoint.X, sensedGridPoint.Y);
        sectionPointCalculator.setSection(&carGridPos, &sensedGridPoint, 1);

        // iterates through section points (points that are between the car and the sensed point) and decrements them
        while (sectionPointCalculator.nextExists()) {
            Point2ui8 gridPoint = static_cast<Point2ui8>(sectionPointCalculator.next());
            if (gridPoint != prevGridPoint)
                grid.decrement(gridPoint.X, gridPoint.Y);
        }
    }
}

Point2ui8 Environment::nextToStream(ByteArray<COMM_MSG_DATA_LENGTH>& result) {
    Point2ui8 firstByte;
    gridWriter.next(result, COMM_MSG_DATA_LENGTH, &firstByte.X, &firstByte.Y);
    return firstByte;
}
