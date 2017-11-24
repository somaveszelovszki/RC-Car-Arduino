#ifndef RC_CAR__ENVIRONMENT__HPP
#define RC_CAR__ENVIRONMENT__HPP

#include "Point2.hpp"
#include "BoolGrid.hpp"

namespace rc_car {

	/** @brief Calculates environment around car from the measured data of the ultrasonic sensors.
	*/
	class Environment {
	private:
        /** @brief Position of the car relative to its start position.
        */
		Point2f carPos;

        /** @brief Car's forward angle in [rad].
        */
        float carFwdAngle_Rad;

        /** @brief Car's forward angle's cosine.
        */
        float carFwdAngle_Cos;

        /** @brief Car's forward angle's sine.
        */
        float carFwdAngle_Sin;

        /** @brief Boolean grid indicating for each point if there is an obstacle there.
        */
		BoolGrid<ENV_ABS_AXIS_POINTS_NUM, ENV_ABS_AXIS_POINTS_NUM> envGrid;

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

            @param _startPoint The starting point of the section.
            @param _endPoint The end point of the section.
            */
			void setSection(const Point2f& _startPoint, const Point2f& _endPoint);

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

	public:
        /** @brief Constructor - resets grid.
        */
		Environment();

        /** @brief Converts relative point to grid point.
        Relative points' coordinates are relative to the car's current position.
        Grid point coordinates are indexes of the environment grid (2-dimensional binary array).

        @param relPoint The relative point.
        @returns The grid point.
        */
        Point2i relPointToGridPoint(const Point2f& relPoint) const;

        /** @brief Sets section parameters.

        @param _startPoint The starting point of the section.
        @param _endPoint The end point of the section.
        */
		void setSection(const Point2f& _startPoint, const Point2f& _endPoint) {
			sectionPointCalculator.setSection(_startPoint, _endPoint);
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

        /** @brief Sets car position.

        @param _carPos The car position to set.
        */
		void setCarPosition(const Point2f _carPos) {
			carPos = _carPos;
		}

        /** @brief Gets car position.

        @returns The car position.
        */
		const Point2f& getCarPosition() const {
			return carPos;
		}

        /** Sets relative point an obstacle - updates point in the environment grid.

        @param relPoint The relative point to set as an obstacle.
        */
		void setRelativePointObstacle(const Point2f& relPoint);

        /** @brief Gets grid point at the given coordinates.

        @param x The X coordinate.
        @param y The y coordinate.
        @returns The grid point's value.
        */
		bool getGridPoint(int x, int y) const {
			return envGrid.get(x, y);
		}

        /** Checks if relative point is an obstacle - checks point in the environment grid.

        @param relPoint The relative point to check for an obstacle.
        @returns Boolean value indicating if the relative point is an obstacle.
        */
		bool isRelativePointObstacle(const Point2f& relPoint) const;

#if __DEBUG
        /** @brief Prints grid points to the Serial port.
        */
		void print() const {
			for (int y = 0; y < ENV_ABS_AXIS_POINTS_NUM; ++y) {
				for (int x = 0; x < ENV_ABS_AXIS_POINTS_NUM; ++x)
					if (x == ENV_ABS_AXIS_POINTS_NUM / 2 && y == ENV_ABS_AXIS_POINTS_NUM / 2)
						DEBUG_print("X");
					else
						DEBUG_print(String(getGridPoint(x, y) ? "1" : "0"));
				DEBUG_println();
			}
			DEBUG_println();
		}
#endif // __DEBUG
	};
}

#endif // RC_CAR__ENVIRONMENT__HPP
