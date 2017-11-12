#ifndef RC_CAR__ENVIRONMENT__HPP
#define RC_CAR__ENVIRONMENT__HPP

#include "Point.hpp"
#include "Vec.hpp"

namespace rc_car {
	/** @brief Calculates environment around car from the measured data of the ultrasonic sensors.
	*/
	class Environment {
	private:
		Point<float> startPoint, diff;

		// Section points include the start point but not the endpoint!
		int sectionPointsCount, currentSectionPointIdx;

		//Vec<Point<float>, ENV_SECTION_POINTS_MAX_NUM> envPoints[ENV_SECTIONS_NUM];
		//void calculateSectionPoints(Point<float> measuredPoints[ULTRA_NUM_SENSORS], Common::UltrasonicPos startPos, int sectionIdx);

	public:
		void setSection(const Point<float>& _startPoint, const Point<float>& _endPoint);

		bool nextSectionPointExists() const {
			return currentSectionPointIdx < sectionPointsCount;
		}

		Point<float> calculateNextSectionPoint();

		//void calculate(Point<float> measuredPoints[ULTRA_NUM_SENSORS], Common::UltrasonicPos startPos);
	};
}

#endif // RC_CAR__ENVIRONMENT__HPP
