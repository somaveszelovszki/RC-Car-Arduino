#include "PD_Controller.hpp"

using namespace rc_car;

float PD_Controller::update(const float& error) {
	unsigned long d_time = Common::milliSecs() - prevTime;
	float derivative = (error - prevError) / d_time;

	prevError = error;
	output = output + error * Kp + derivative * Kd;

	prevTime = Common::milliSecs();

	return getOutput();
}

