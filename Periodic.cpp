#include "Periodic.hpp"

void rc_car::Periodic::run(void *data = NULL) {
	__run(data);
	restartPeriodCheck();
}
