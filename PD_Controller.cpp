#include "PD_Controller.hpp"

using namespace rc_car;

void PD_Controller::run(float currentError) {
	unsigned long d_time = millis() - getPrevCalledTime();
	float derivative = (currentError - prevError) / d_time;

	//DEBUG_println("currentError: " + String(currentError));
	//DEBUG_println("prevError: " + String(prevError));
	//DEBUG_println("derivative: " + String(derivative));

	//DEBUG_println("prev output: " + String(output));


	output += currentError * Kp + derivative * Kd;
	//output += currentError * Kp;
	//DEBUG_println("new output: " + String(output));
	//DEBUG_println();

	if (output > DC_MAX_VALUE) output = static_cast<float>(DC_MAX_VALUE);
	else if (output < DC_MIN_VALUE) output = static_cast<float>(DC_MIN_VALUE);

	prevError = currentError;
}
