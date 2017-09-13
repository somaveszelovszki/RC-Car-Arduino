#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Point.hpp"
#include "Vec.hpp"

typedef Vec<Point<float>, ENV_SECTION_POINTS_MAX_NUM> EnvSectionVec;

class Environment {
private:
	void calculateSectionPoints(Common::UltrasonicPos pos, EnvSectionVec& dest);

public:
	Common::UltrasonicPos fwdDir;
	Point<float> points[ULTRA_NUM_SENSORS];
	EnvSectionVec combinedSections[ENV_COMBINED_SECTIONS_NUM][ENV_COMBINED_SECTION_SENSORS_NUM];

	void calculateCombinedSections();
};

#endif // ENVIRONMENT_HPP
