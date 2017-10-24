#include "PD_Controller.hpp"

using namespace rc_car;

void PD_Controller::__run(void *pError) {
	float currentError = *(static_cast<float*>(pError));
	unsigned long d_time = millis() - getPrevCalledTime();
	float derivative = (currentError - prevError) / d_time;

	output = output + currentError * Kp + derivative * Kd;
	prevError = currentError;
}

