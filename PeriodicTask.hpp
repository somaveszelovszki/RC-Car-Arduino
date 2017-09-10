#ifndef PERIODIC_TASK_HPP
#define PERIODIC_TASK_HPP

#include "Periodic.hpp"

template <typename P, typename R>
class PeriodicTask : public Periodic {

private:
	virtual R __run(const P& param) = 0;

public:
	PeriodicTask(int _periodTime) : Periodic(_periodTime) {}

	R run(const P& param) {
		R result = __run(param);
		prevTime = Common::milliSecs();
		return result;
	}
};

#endif // PERIODIC_TASK_HPP
