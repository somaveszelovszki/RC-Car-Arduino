#ifndef RC_CAR__ENVIRONMENT__HPP
#define RC_CAR__ENVIRONMENT__HPP

#include "Point2.hpp"
#include "Vec.hpp"
#include "BoolGrid.hpp"

namespace rc_car {

#define ABS_COORD_TO_GRID_POS(coord) static_cast<int>(coord > 0 ? (coord + (ENV_ABS_POINTS_DIST / 2)) / ENV_ABS_POINTS_DIST : (coord - (ENV_ABS_POINTS_DIST / 2)) / ENV_ABS_POINTS_DIST)

	/** @brief Calculates environment around car from the measured data of the ultrasonic sensors.
	*/
	class Environment {
	private:
		Point2f carPos;
		float carFwdDir_Rad, carFwdDir_Cos, carFwdDir_Sin;

		BoolGrid<ENV_ABS_AXIS_POINTS_NUM, ENV_ABS_AXIS_POINTS_NUM> envGrid;

		class SectionPointCalculator {
		private:
			Point2f startPoint, diff;

			// Section points include the start point but not the endpoint!
			int pointsCount, currentPointIdx;

		public:
			void setSection(const Point2f& _startPoint, const Point2f& _endPoint);

			bool nextExists() const {
				return currentPointIdx < pointsCount;
			}

			Point2f next() {
				return startPoint + diff * currentPointIdx++;
			}
		};

		SectionPointCalculator sectionPointCalculator;

	public:
		Environment();

		Point2i relPointToGridPoint(const Point2f& relPoint) {

			Point2f absPos = carPos + Point2f(
				relPoint.X * carFwdDir_Cos - relPoint.Y * carFwdDir_Sin,
				relPoint.X * carFwdDir_Sin + relPoint.Y * carFwdDir_Cos);

			return Point2i(ABS_COORD_TO_GRID_POS(absPos.X), ABS_COORD_TO_GRID_POS(absPos.Y));
		}

		void setSection(const Point2f& _startPoint, const Point2f& _endPoint) {
			sectionPointCalculator.setSection(_startPoint, _endPoint);
		}

		bool nextSectionPointExists() const {
			return sectionPointCalculator.nextExists();
		}

		Point2f nextSectionPoint() {
			return sectionPointCalculator.next();
		}

		void setCarPosition(const Point2f _carPos) {
			carPos = _carPos;
		}

		const Point2f& getCarPosition() const {
			return carPos;
		}

		void setRelativePointObstacle(const Point2f& relPoint);

		bool getGridPoint(int x, int y) const {
			return envGrid.get(x, y);
		}

		bool isRelativePointObstacle(const Point2f& relPoint) const;

#if __DEBUG
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
