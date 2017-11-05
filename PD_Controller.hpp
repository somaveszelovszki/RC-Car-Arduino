#ifndef PD_CONTROLLER_HPP
#define PD_CONTROLLER_HPP

#include "PeriodicTask.hpp"

namespace rc_car {
	/** @brief PD controller implementation.
	*/
	class PD_Controller : public Periodic {
	private:
		float Kp, Kd;
		float prevError = 0.0f;
		float output = 0.0f;

	public:

		PD_Controller(int _periodTime, float _Kp, float _Kd) : Periodic(_periodTime), Kp(_Kp), Kd(_Kd) {}

		void run(float currentError);

		float getOutput() const {
			return output;
		}
	};
}

#endif // PD_CONTROLLER_HPP