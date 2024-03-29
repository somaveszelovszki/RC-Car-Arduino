#ifndef RC_CAR__ENVIRONMENT__HPP
#define RC_CAR__ENVIRONMENT__HPP

#include "CarProps.hpp"
#include "Grid.hpp"
#include "Message.hpp"

namespace rc_car {

/** @brief Converts X coordinate to grid index.
*/
#define COORD_TO_GRID_X(coord) (ENV_GRID_AXIS_NUM_POINTS / 2 + coord / ENV_GRID_DIST)

/** @brief Converts Y coordinate to grid index.
*/
#define COORD_TO_GRID_Y(coord) (ENV_GRID_AXIS_NUM_POINTS / 2 - coord / ENV_GRID_DIST)

/** @brief Converts grid X index to coordinate.
*/
#define GRID_TO_COORD_X(pos) ((pos - ENV_GRID_AXIS_NUM_POINTS / 2) * ENV_GRID_DIST)

/** @brief Converts grid Y index to coordinate.
*/
#define GRID_TO_COORD_Y(pos) ((ENV_GRID_AXIS_NUM_POINTS / 2 - pos) * ENV_GRID_DIST)

/** @brief Calculates environment around car from the measured data of the ultrasonic sensors.
*/
class Environment {
private:
    /** @brief Pointer to the structure storing the car properties (speed, steering angle, etc).
    */
    const CarProps *pCar;

    /** @brief Grid position of the car.
    */
    Point2ui8 carGridPos;

    /** @brief The grid type.
    */
    typedef Grid<ENV_GRID_POINT_BIT_DEPTH, ENV_GRID_AXIS_NUM_POINTS, ENV_GRID_AXIS_NUM_POINTS> grid_type;

    /** @brief Boolean grid indicating for each point if there is an obstacle there.
    */
    grid_type grid;

    grid_type::StreamWriter gridWriter;

    /** @brief Array containing the grid points of the current section.
    */
    Point2ui8 sectionGridPoints[ENV_ABS_SECTION_POINTS_MAX_NUM];

    /** @brief Calculates section points.
    Sections are a subset of the environment points - they are between two sensed points.
    The whole set of sections equals the environment points' set.
    The number of sections is equal to the number of ultrasonic sensors.

    NOTE: The starting point is included in the section but the end point is not!
    */
    class SectionPointCalculator {
    private:
        /** @brief The starting point for the section.
        */
        Point2f startPoint;

        /** @brief The difference between the section points.
        */
        Vec2f diff;

        /** @brief Number of points in the section.
        */
        int pointsCount;

        /** @brief Index of current section point - used when iterating through all section points.
        */
        int currentPointIdx;

    public:
        /** @brief Sets section parameters.

        @param pStartPoint Pointer to the starting point of the section.
        @param pEndPoint Pointer to the end point of the section.
        @param targetDiff Distance of section points. NOTE: If using this difference results in more section points than the maximum, will not be used!
        @param maxPointsNum Maximum number of section points (0 means no limit) - 0 by default.
        */
        void setSection(const Point2f *pStartPoint, const Point2f *pEndPoint, float targetDiff, uint8_t maxPointsNum = 0);

        /** @brief Checks if there still exists a point in the section that has not been acquired.

        @returns Boolean value indicating if there still exists a point in the section that has not been acquired.
        */
        bool nextExists() const {
            return currentPointIdx < pointsCount;
        }

        /** @brief Gets next secion point.

        @returns The next section point.
        */
        Point2f next() {
            return startPoint + diff * currentPointIdx++;
        }
    };

    /** @brief Used for calculating section points.
    */
    SectionPointCalculator sectionPointCalculator;

    const Point2f *sensedPoints;

public:
    /** @brief Constructor - resets grid and sets sensed points array.

    @param _pCar Pointer to the structure storing the car properties.
    @param _sensedPoints Array of the sensed points (provided by the UltrasonicTask).
    */
    Environment(const CarProps *_pCar, const Point2f *_sensedPoints);

    /** @brief Converts relative point to grid point.
    Relative points' coordinates are relative to the car's current position.
    Grid point coordinates are indexes of the environment grid (2-dimensional binary array).

    @tparam Numeric type of the result point type - uint8_t by default.
    @param relPoint The relative point.
    @returns The grid point.
    */
    template <typename T = uint8_t>
    Point2<T> relPointToGridPoint(const Point2f& relPoint) const {
        // car pos + pos rotated with (fwdAngle - 90 degrees)
        Point2f absPos = pCar->pos + Point2f(
            relPoint.X * pCar->fwdAngle_Sin + relPoint.Y * pCar->fwdAngle_Cos,
            -relPoint.X * pCar->fwdAngle_Cos + relPoint.Y * pCar->fwdAngle_Sin);

        return Point2<T>(toRealIdx<T>(COORD_TO_GRID_X(absPos.X)), toRealIdx<T>(COORD_TO_GRID_Y(absPos.Y)));
    }

    /** @brief Sets section parameters.

    @param pStartPoint Pointer to the starting point of the section.
    @param pEndPoint Pointer to the end point of the section.
    @param targetDiff Distance of section points. NOTE: If using this difference results in more section points than the maximum, will not be used!
    @param maxPointsNum Maximum number of section points.
    */
    void setSection(const Point2f *pStartPoint, const Point2f *pEndPoint, float targetDiff, uint8_t maxPointsNum) {
        sectionPointCalculator.setSection(pStartPoint, pEndPoint, targetDiff, maxPointsNum);
    }

    /** @brief Checks if there still exists a point in the section that has not been acquired.

    @returns Boolean value indicating if there still exists a point in the section that has not been acquired.
    */
    bool nextSectionPointExists() const {
        return sectionPointCalculator.nextExists();
    }

    /** @brief Gets next secion point.

    @returns The next section point.
    */
    Point2f nextSectionPoint() {
        return sectionPointCalculator.next();
    }

    /** @brief Gets grid point at the given coordinates.

    @param x The X coordinate.
    @param y The y coordinate.
    @returns The grid point's value.
    */
    bool getGridPoint(int x, int y) const {
        return grid.get(x, y);
    }

    /** @brief Checks if grid index is out of bounds, and if yes, returns out-of-bounds index.

    @tparam T Numeric type of the result.
    @param idx The unchecked grid index.
    @param gridResRate Rate of the current grid resolution and the environment grid resolution - 1 by default.
    @returns The checked grid index.
    */
    template <typename T>
    static T toRealIdx(float idx, uint8_t gridResRate = 1) {
        float _idx = idx * gridResRate, _max = ENV_GRID_AXIS_NUM_POINTS * gridResRate;
        return static_cast<T>((idx < 0.0f || _idx >= _max) ? -1 : _idx);
    }

    /** @brief Calculates car grid coordinates with a given resolution.
    When sending car coodinates, it is good to have a higher resolution so that the car's movement is flawless.

    @tparam T Numeric type of the result point - uint8_t by default.
    @param gridResRate Rate of the car grid resolution and the environment grid resolution - 1 by default.
    @returns The car grid coordinates.
    */
    template <typename T = uint8_t>
    Point2<T> getCarGridCoords(uint8_t gridResRate = 1) const {
        return Point2<T>(toRealIdx<T>(COORD_TO_GRID_X(pCar->pos.X), gridResRate), toRealIdx<T>(COORD_TO_GRID_Y(pCar->pos.Y), gridResRate));
    }

    /** Checks if relative point is an obstacle - checks point in the environment grid.

    @param relPoint The relative point to check for an obstacle.
    @returns Boolean value indicating if the relative point is an obstacle.
    */
    bool isRelativePointObstacle(const Point2f& relPoint) const;

    /** @brief Updates environment grid.
    For points that has been sensed as obstacles, increments obstacle probability,
    for points that are between the car and the sensed points (where there is no obstacle) decrements obstacle probability.
    */
    void updateGrid();

    /** @brief Writes next segment to byte array.

    @param result The result byte array.
    @returns Point containing the X and Y coordinates of the first written byte.
    */
    Point2ui8 nextToStream(ByteArray<COMM_MSG_DATA_LENGTH>& result);

    /** @brief Gets environment grid message code from grid coordinates.

    @param coords The grid coordinates.
    @returns The code byte.
    */
    static byte gridCoordsToCodeByte(const Point2ui8& coords) {
        return Message::CODES[Message::CODE::EnvGrid].codeByte + coords.Y;
    }

    /** @brief Gets grid coordinates from environment grid message code.

    @param codeByte The code byte.
    @returns The grid coordinates.
    */
    static Point2ui8 codeByteToGridCoords(byte codeByte) {
        return Point2ui8((codeByte & 0b00110000) >> 4, codeByte & 0b00001111);
    }

#if __DEBUG__
    /** @brief Prints grid points to the Serial port.
    */
    void print() const {
        grid.print();
    }
#endif // __DEBUG__
};
}

#endif // RC_CAR__ENVIRONMENT__HPP