#ifndef PD_CONTROLLER_HPP
#define PD_CONTROLLER_HPP

#include "PeriodicTask.hpp"

class PD_Controller : public PeriodicTask<float, int> {
private:
	float Kp, Kd;
	float prevError = 0.0f;
	float output = 0.0f;

	virtual int __run(const float& error) override;

public:

	PD_Controller(int _periodTime, float _Kp, float _Kd) : PeriodicTask(_periodTime), Kp(_Kp), Kd(_Kd) {}

	float getOutput() const {
		return output;
	}
};

#endif // PD_CONTROLLER_HPP