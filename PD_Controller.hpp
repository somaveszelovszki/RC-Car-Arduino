#ifndef RC_CAR__PD_CONTROLLER__HPP
#define RC_CAR__PD_CONTROLLER__HPP

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

		int getOutput() const {
			return static_cast<int>(output);
		}
	};
}

#endif // RC_CAR__PD_CONTROLLER__HPP