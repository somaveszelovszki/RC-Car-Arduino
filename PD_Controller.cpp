#include "PD_Controller.hpp"

using namespace rc_car;

void PD_Controller::run(float currentError) {
	unsigned long d_time = millis() - getPrevCalledTime();
	float derivative = (currentError - prevError) / d_time;

    output = Common::incarcerate(output + currentError * Kp + derivative * Kd, outMin, outMax);
	prevError = currentError;
}
