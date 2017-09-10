#include "PD_Controller.hpp"

int PD_Controller::__run(const float& error) {

	unsigned long d_time = Common::milliSecs() - prevTime;
	float derivative = (error - prevError) / d_time;

	prevError = error;
	output = output + error * Kp + derivative * Kd;

	return static_cast<int>(output);
}

