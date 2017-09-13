#include "Environment.hpp"

void Environment::calculateSectionPoints(Common::UltrasonicPos pos, EnvSectionVec& dest) {
	Point<float> start = points[pos], end = points[(pos + 1) % ULTRA_NUM_SENSORS];


	Point<float> diff = (end - start) / ENV_SECTION_POINTS_MAX_NUM;
	dest.length = ENV_SECTION_POINTS_MAX_NUM;

	float len = diff.length();
	if (len < ENV_SECTION_POINTS_MIN_DIST) {
		diff *= ENV_SECTION_POINTS_MIN_DIST / len;
		dest.length *= len / ENV_SECTION_POINTS_MIN_DIST;
	}
	
	for (int i = 0; i < dest.length; ++i)
		dest[i] = start + i * diff;
}

void Environment::calculateCombinedSections() {
	for (int i = 0; i < ULTRA_NUM_SENSORS; ++i) {
		calculateSectionPoints(static_cast<Common::UltrasonicPos>((fwdDir + i) % ULTRA_NUM_SENSORS),
			combinedSections[i / ENV_COMBINED_SECTION_SENSORS_NUM][i % ENV_COMBINED_SECTION_SENSORS_NUM]);
	}
}
